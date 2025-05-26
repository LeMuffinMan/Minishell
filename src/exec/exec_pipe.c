/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:24:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:24:29 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "exec.h"
#include "malloc_error_handlers.h"
#include "signals.h"
#include "structs.h"
#include <errno.h>
#include <unistd.h>

int	exec_pipe(t_tree **ast, t_lists *lists)
{
	pid_t				left_pid;
	int					exit_code;
	struct sigaction	sa_ignore;
	struct sigaction	sa_orig;

	setup_pipe_signals(&sa_ignore, &sa_orig);
	add_pipe(lists->pipe_fd, lists->pipes);
	if (errno == ENOMEM)
		return (errno);
	left_pid = fork();
	if (exec_pipe_left_execution(ast, lists, left_pid) == -1)
		return (malloc_error_close_free_pipes(lists->pipe_fd, lists->pipes));
	if ((*ast)->right && (*ast)->right->token->token == PIPE)
	{
		exit_code = exec_pipe_right_pipe_execution(ast, lists, left_pid);
		if (handle_pipe_error(lists))
			return (errno);
		return (exit_code);
	}
	return (handle_right_execution(ast, lists, left_pid, &sa_orig));
}

int	exec_pipe_left_execution(t_tree **ast, t_lists *lists, pid_t pid)
{
	if (pid < 0)
		return (errno);
	if (pid == 0)
		left_child_execution(ast, lists);
	else
	{
		if (left_parent_execution(lists->pipes, lists->pipe_fd) == -1)
			return (-1);
	}
	return (0);
}

int	exec_pipe_right_execution(t_tree **ast, t_lists *lists, pid_t right_pid,
		pid_t left_pid)
{
	if (right_pid < 0)
		return (errno);
	if (right_pid == 0)
		right_child_execution(ast, lists, lists->pipe_fd, lists->pipes);
	else
	{
		if (close(lists->pipe_fd[0]) == -1)
			return (-1);
		free_pipes(lists->pipes);
		lists->exit_code = wait_children(right_pid, left_pid);
		return (lists->exit_code);
	}
	return (0);
}

int	exec_pipe_right_pipe_execution(t_tree **ast, t_lists *lists, pid_t left_pid)
{
	int	exit_code;

	exit_code = exec_pipe(&((*ast)->right), lists);
	if (errno == ENOMEM)
		return (errno);
	exit_code = wait_children(left_pid, left_pid);
	return (exit_code);
}

int	handle_right_execution(t_tree **ast, t_lists *lists, pid_t left_pid,
		struct sigaction *sa_orig)
{
	pid_t	right_pid;
	int		exit_code;

	right_pid = fork();
	exit_code = exec_pipe_right_execution(ast, lists, right_pid, left_pid);
	if (handle_pipe_error(lists))
		return (errno);
	sigaction(SIGINT, sa_orig, NULL);
	update_last_arg_var(lists->env, (*ast)->token->content);
	if (errno == ENOMEM)
		return (errno);
	return (exit_code);
}
