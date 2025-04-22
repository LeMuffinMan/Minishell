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

/* int add_pid(pid_t new_pid, t_pids **pids) */
/* { */
/*     t_pids *new_node; */
/*     t_pids *last; */
/**/
/*     new_node = malloc(sizeof(t_pids)); */
/*     if (!new_node) */
/*         return (-1); */
/*     new_node->pid = new_pid; */
/*     new_node->next = NULL; */
/*     if (!pids && !*pids) */
/*         *pids = new_node; */
/*     else */
/*     { */
/*         last = *pids; */
/*         while (last && last->next != NULL) */
/*             last = last->next; */
/*         last->next = new_node; */
/*     } */
/*     return (0); */
/* } */

int	add_pipe(int fd[2], t_pipe **pipes)
{
	t_pipe	*new_pipe;

	if (pipe(fd) == -1)
	{
		// error
	}
	new_pipe = malloc(sizeof(t_pipe));
	if (!new_pipe)
	{
		// error
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

/* int	free_pids(t_pids **pids) */
/* { */
/* 	t_pids	*tmp; */
/**/
/* 	while (*pids && (*pids)->next) */
/* 	{ */
/* 		tmp = *pids; */
/* 		*pids = (*pids)->next; */
/* 		free(tmp); */
/* 	} */
/* 	*pids = NULL; */
/* 	return (0); */
/* } */

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
