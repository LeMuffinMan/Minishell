/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_fork_and_signals_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:21:19 by oelleaum          #+#    #+#             */
/*   Updated: 2025/06/04 17:29:52 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "signals.h"
#include "token.h"
#include "parse_and_execution_loop.h"
#include "here_doc.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	here_doc_readline_signals_handler(char **line, char *limiter, int fd)
{
	*line = readline("> ");
	if (g_signal == 130)
	{
		if (limiter)
			free(limiter);
		close(fd);
		return (1);
	}
	if (!*line)
	{
		limiter[ft_strlen(limiter) - 1] = '\0';
		printf("minishell: here-doc delimited by end-of-file (wanted `%s')\n",
			limiter);
		return (-1);
	}
	return (0);
}

int	here_doc_child(t_lists *lists, t_token *node, int fd, char *limiter)
{
	setup_here_doc_signals();
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	free_parse(node);
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
	if (g_signal == 130)
		exit(130);
	free(limiter);
	close(fd);
	exit(EXIT_SUCCESS);
}

pid_t	manage_here_doc_fork(int fd, t_lists *lists, char *limiter,
		t_token *node)
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
		here_doc_child(lists, node, fd, limiter);
	setup_parent_signals();
	return (pid);
}

int	open_and_fill_here_docs(t_token *tmp, bool sig_hd, t_lists *lists,
		t_token **head)
{
	while (tmp)
	{
		if (tmp->token == HD && sig_hd == true)
		{
			if (!create_here_doc(tmp, lists, &sig_hd))
			{
				free_parse(*head);
				if (!(lists->exit_code == 130))
					errno = MEM_ALLOC;
				return (1);
			}
			if (g_signal == 130)
			{
				free_parse(*head);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
