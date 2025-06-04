/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/01 14:55:03 by oelleaum         ###   ########lyon.fr   */
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
		if (g_signal == 130)
		{
			free(limiter);
			close(fd);
			exit(130);
		}
		if (!line)
		{
			printf(
				"minishell: here-doc delimited by end-of-file (wanted `%s')\n",
				limiter);
			break ;
		}
		tmp = line;
		line = ft_strjoin(line, "\n");
		if (errno == ENOMEM)
			return (false);
		free(tmp);
		len = ft_strlen(limiter);
		if (case_is_limiter(line, limiter, fd, len))
			break ;
		free(line);
	}
	return (true);
}

pid_t	manage_here_doc_fork(int fd, t_lists *lists,
			char *limiter, t_token *node)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		close(fd);
		free(limiter);
		return (-1);
	}
	if (pid == 0)
	{
		setup_here_doc_signals();
		close_origin_fds(lists->origin_fds);
		free_lists(lists);
		free_parse(node, NULL, 0);
		if (!extract_stdin(fd, limiter))
		{
			if (errno == ENOMEM)
			{
				free(limiter);
				close(fd);
				exit(errno);
			}
			free(limiter);
			close(fd);
			exit(EXIT_FAILURE);
		}
		free(limiter);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	setup_parent_signals();
	return (pid);
}

bool	wait_here_doc(pid_t pid, t_lists *lists, bool *sig_hd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		lists->exit_code = WEXITSTATUS(status);
		if (lists->exit_code == 130)
			*sig_hd = false;
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			lists->exit_code = 130; // a virer ?
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
	if (*sig_hd == true)
		pid = manage_here_doc_fork(fd, lists, limiter, node);
	else
		return (false);
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

	sig_hd = true;
	if (!verif_here_doc(head))
		return (false);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == HD && sig_hd == true)
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
	/* *sig_hd = true; */
		tmp = tmp->next;
	}
	return (true);
}
