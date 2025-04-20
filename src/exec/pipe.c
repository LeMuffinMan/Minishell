/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:33:31 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:51:08 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "libft.h"
#include <unistd.h>

int	wait_children(pid_t last_child, t_pids **pids)
{
	t_pids *tmp;
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	if (pids)
		tmp = *pids;
	else
		tmp = NULL;
	while(tmp && tmp->next)
	{
		waitpid(tmp->pid, &status, 0);
		tmp = tmp->next;
	}
	waitpid(last_child, &status, 0);
	//ici on free la liste des pids
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

int add_pid(pid_t new_pid, t_pids **pids)
{
	t_pids *new_node; 
	t_pids *last;

	last = NULL;
	new_node = malloc(sizeof(t_pids));
	if (!new_node)
	{
		//malloc error
	}
	if (!*pids)
		*pids = new_node;
	else
	{
		last = *pids;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
	new_node->next = NULL;
	new_node->pid = new_pid;
	return (0);
}

int add_pipe(int fd[2], t_pipe **pipes)
{
	t_pipe *new_pipe;

	if (pipe(fd) == -1)
	{
		//error
	}
	new_pipe = malloc(sizeof(t_pipe));
	if (!new_pipe)
	{
		//error
	}
	new_pipe->next = *pipes;
	new_pipe->prev = NULL;
	new_pipe->fd[0] = fd[0];
	new_pipe->fd[1] = fd[1];
	if (*pipes)
		(*pipes)->prev = new_pipe;
	*pipes = new_pipe; 
	return (0);
}

int free_pids(t_pids **pids)
{
	t_pids *tmp;

	while(*pids && (*pids)->next)
	{
		tmp = *pids;
		*pids = (*pids)->next;
		free(tmp);
	}
	return (0);
}

int free_pipes(t_pipe **pipes)
{
	t_pipe *tmp;

	tmp = *pipes;
	while (*pipes && (*pipes)->next)
	{
		tmp = *pipes;
		*pipes = (*pipes)->next;
		free(tmp);
	}
	free(*pipes);
	pipes = NULL;
	return (0);
}


