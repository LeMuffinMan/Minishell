/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 19:45:24 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>
#include "signals.h"
#include <sys/stat.h>

bool	verif_name(char *name)
{
	struct stat	status;

	if (stat(name, &status) == 0)
	{
		free(name);
		return (false);
	}
	return (true);
}

bool	extract_stdin(int fd, char *limiter)
{
	int		len;
	char	*line;
	char *tmp;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", limiter);
			break ;
		}
		tmp = line;
		line = ft_strjoin(line, "\n");
		free(tmp);
		len = ft_strlen(limiter);
		if (ft_strncmp(line, limiter, len) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (false);
		}
		free(line);
	}
	return (true);
}

bool	create_here_doc(t_token *node, t_lists *lists)
{
	int		fd;
	char	*limiter;
	pid_t pid;
	struct sigaction sa_ignore, sa_orig;

	fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (false);
	limiter = ft_strdup(node->content[1]);
	if (!limiter)
	{
		close(fd);
		return (false);
	}
	setup_pipe_signals(&sa_ignore, &sa_orig);
	pid = fork();
	if (pid < 0)
	{
		//on quit tout
	}
	if (pid == 0)
	{
		setup_child_signals();
		if (!extract_stdin(fd, limiter))
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else
	{
		//proteger le wait
		lists->exit_code = wait_children(pid, pid);
		sigaction(SIGINT, &sa_orig, NULL);
	}
	close(fd);
	free(limiter);
	return (true);
}

bool	handle_here_doc(t_token **head, t_lists *lists)
{
	t_token	*tmp;

	if (!head || !*head)
		return (false);
	if (!create_hd_name(head))
		return (false);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == HD)
		{
			if (!create_here_doc(tmp, lists))
				return (false);
		}
		tmp = tmp->next;			
	}
	return (true);
}
