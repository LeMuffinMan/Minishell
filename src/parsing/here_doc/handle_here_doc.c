/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 17:16:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include "signals.h"
#include <sys/stat.h>
#include <errno.h>

bool	extract_stdin(int fd, char *limiter)
{
	int		len;
	char	*line;
	char	*tmp;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf(
				"minishell: here-doc delimited by end-of-file (wanted `%s')\n",
				limiter);
			break ;
		}
		tmp = line;
		line = ft_strjoin(line, "\n");
		free(tmp);
		len = ft_strlen(limiter);
		if (case_is_limiter(line, limiter, fd, len))
			break ;
		else
			return (false);
		free(line);
	}
	return (true);
}

pid_t	manage_here_doc_fork(int fd, t_lists *lists,
			char *limiter, t_token *node)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(fd);
		free(limiter);
		return (-1);
	}
	if (pid == 0)
	{
		close_origin_fds(lists->origin_fds);
		free_lists(lists);
		free_parse(node, NULL, 0);
		if (!extract_stdin(fd, limiter))
		{
			free(limiter);
			close(fd);
			exit(EXIT_FAILURE);
		}
		free(limiter);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	return (pid);
}

bool	wait_here_doc(pid_t pid, t_lists *lists, bool *sig_hd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		lists->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			*sig_hd = 0;
			lists->exit_code = 130;
			write(1, "\n", 1);
			return (false);
		}
		else
			lists->exit_code = 1;
	}
	return (true);
}

bool	create_here_doc(t_token *node, t_lists *lists, bool *sig_hd)
{
	int		fd;
	char	*limiter;
	pid_t	pid;

	fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (false);
	limiter = get_limiter(node->content[1], fd);
	if (!limiter)
		return (false);
	pid = manage_here_doc_fork(fd, lists, limiter, node);
	if (pid < 0)
		return (false);
	else
	{
		if (!free_lim_close_fd(limiter, fd))
			return (false);
		if (!wait_here_doc(pid, lists, sig_hd))
			return (false);
	}
	return (true);
}

bool	handle_here_doc(t_token **head, t_lists *lists)
{
	t_token	*tmp;
	bool	sig_hd;

	sig_hd = 1;
	if (!verif_here_doc(head))
		return (false);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == HD && sig_hd)
		{
			if (!create_here_doc(tmp, lists, &sig_hd))
			{
				free_parse(*head, NULL, MEM_ALLOC);
				if (lists->exit_code == 130)
					errno = 130;
				else
					errno = MEM_ALLOC;
				return (false);
			}
		}
		tmp = tmp->next;
	}
	return (true);
}
