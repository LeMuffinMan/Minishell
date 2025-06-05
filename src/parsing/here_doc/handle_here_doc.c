/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:11:15 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "libft.h"
#include "list.h"
#include "signals.h"
#include "token.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

bool	extract_stdin(int fd, char *limiter)
{
	int		len;
	char	*line;
	char	*tmp;
	int		exit_code;

	exit_code = 0;
	while (1)
	{
		exit_code = here_doc_readline_signals_handler(&line, limiter, fd);
		if (exit_code == -1)
			break ;
		else if (exit_code == 1)
			return (true);
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
	if (open_and_fill_here_docs(tmp, sig_hd, lists, head))
		return (false);
	return (true);
}
