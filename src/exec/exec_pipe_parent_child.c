

#include <stdio.h>
#include <errno.h>
#include "malloc_error_handlers.h"
#include "exec.h"
#include "signals.h"
#include "utils.h"
#include <unistd.h>
#include <stdlib.h>
#include "structs.h"

int left_child_execution_stdio(t_lists *lists)
{
	if (close_origin_fds(lists->origin_fds) == -1)
		return (-1);
	if (close(lists->pipe_fd[0]) == -1)
		return (-1);
	if ((*lists->pipes)->next)
	{
		if (dup2((*lists->pipes)->next->fd[0], STDIN_FILENO) == -1)
			return (-1);
		if (close((*lists->pipes)->next->fd[0]) == -1)
			return (-1);
		free((*lists->pipes)->next);
		(*lists->pipes)->next = NULL;
	}
	if (dup2(lists->pipe_fd[1], STDOUT_FILENO) == -1)
		return (-1);
	if (close(lists->pipe_fd[1]) == -1)
		return (-1);
	free_pipes(lists->pipes);
	return (0);
}


int left_child_execution(t_tree **ast, t_lists *lists)
{
	int exit_code;

	exit_code = 0;
	if (left_child_execution_stdio(lists) == -1)
		malloc_error_freelists_exit(lists);
	exit_code = exec_ast(&(*ast)->left, lists);
	if (errno == ENOMEM)
		malloc_error_freelists_exit(lists);
	free_lists(lists);
	exit(exit_code);
	return (0);
}

int left_parent_execution(t_pipe **pipes, int pipefd[2])
{
	if ((*pipes)->next)
	{
		if (close((*pipes)->next->fd[0]) == -1)
			return (-1);
		free((*pipes)->next);
		(*pipes)->next = NULL;
	}
	if (close(pipefd[1]) == -1)
		return (-1);
	return (0);
}

int right_child_execution(t_tree **ast, t_lists *lists, int pipefd[2], t_pipe **pipes)
{
	int exit_code;

	exit_code = 0;
	if (close_origin_fds(lists->origin_fds) == -1)
		malloc_error_freelists_exit(lists);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		malloc_error_freelists_exit(lists);
	if (close(pipefd[0]) == -1)
		malloc_error_freelists_exit(lists);
	free_pipes(pipes);
	exit_code = exec_ast(&((*ast)->right), lists);
	if (errno == ENOMEM) // d'autres code d'erreur ?
		malloc_error_freelists_exit(lists);
	if (close_origin_fds(lists->origin_fds) == -1)
		malloc_error_freelists_exit(lists);
	free_lists(lists);
	exit(exit_code);
	return (0);
}


