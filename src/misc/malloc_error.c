/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:40:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:40:31 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_error_handlers.h"
#include "structs.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void	malloc_error_freelists_exit(t_lists *lists)
{
	int	saved_errno;

	saved_errno = errno;
	free_lists(lists);
	exit(saved_errno);
}

int	malloc_error_close_free_pipes(int pipefd[2], t_pipe **pipes)
{
	int	saved_errno;

	saved_errno = errno;
	if (close(pipefd[0]) == -1 || close(pipefd[1]) == -1)
	{
		free_pipes(pipes);
		return (saved_errno);
	}
	free_pipes(pipes);
	return (saved_errno);
}

int	handle_pipe_error(t_lists *lists)
{
	if (errno == ENOMEM)
		return (malloc_error_close_free_pipes(lists->pipe_fd, lists->pipes));
	return (0);
}

int	malloc_error_parenthesis_child(t_lists *lists, t_tree **ast_to_free)
{
	int	saved_errno;

	saved_errno = errno;
	if (ast_to_free && *ast_to_free)
		free_tree(ast_to_free);
	free_lists(lists);
	exit(saved_errno);
}

int	malloc_free_string(char *s)
{
	int	saved_errno;

	saved_errno = errno;
	if (s)
		free(s);
	return (saved_errno);
}

int	builtin_exit_malloc_error(t_lists *lists, char *s)
{
	int	saved_errno;

	saved_errno = errno;
	if (s)
		free(s);
	free_lists(lists);
	close_origin_fds(lists->origin_fds);
	exit(saved_errno);
}

int	malloc_error_close_free_exit(t_lists *lists)
{
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(errno);
}
