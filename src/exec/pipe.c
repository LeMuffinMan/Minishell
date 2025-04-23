/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:33:31 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:37:38 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <unistd.h>

	// ici on free la liste des pids
int	wait_children(pid_t last_child, pid_t first_child)
{
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	waitpid(first_child, &status, 0);
	waitpid(last_child, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	if (exit_code == EXIT_SUCCESS && WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (exit_code == EXIT_SUCCESS && WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

int	add_pipe(int fd[2], t_pipe **pipes)
{
	t_pipe	*new_pipe;

	if (pipe(fd) == -1)
		return (-1);
	new_pipe = malloc(sizeof(t_pipe));
	if (!new_pipe)
	{
		//error
	}
	if (pipes && *pipes)
		new_pipe->next = *pipes;
	else
		new_pipe->next = NULL;
	new_pipe->fd[0] = fd[0];
	new_pipe->fd[1] = fd[1];
	*pipes = new_pipe;
	return (0);
}

int	free_pipes(t_pipe **pipes)
{
	t_pipe	*tmp;

	if (pipes)
		tmp = *pipes;
	else
		return (1);
	while (*pipes && (*pipes)->next)
	{
		tmp = *pipes;
		*pipes = (*pipes)->next;
		free(tmp);
	}
	free(*pipes);
	*pipes = NULL;
	return (0);
}
