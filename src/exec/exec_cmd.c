/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:23:40 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:24:13 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "exec.h"
#include "libft.h"
#include "malloc_error_handlers.h"
#include "signals.h"
#include "structs.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include "exec_cmd.h"

int	exec_cmd_execve(t_tree **ast, t_lists *lists)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (exec_cmd_execve_child(ast, lists) == -1)
			return (-1);
	}
	else
	{
		set_signals(1);
		exit_code = wait_children(pid, pid);
		update_last_arg_var(lists->env, (*ast)->token->content);
		if (errno == ENOMEM)
			return (errno);
		setup_parent_signals();
		rl_on_new_line();
		return (exit_code);
	}
	return (1);
}

int	exec_cmd(t_tree **ast, t_lists *lists)
{
	int		exit_code;

	if ((*ast)->token->token == BUILT_IN
		|| !ft_strncmp((*ast)->token->content[0], "source", 7))
	{
		exit_code = builtins((*ast)->token->content, lists);
		return (exit_code);
	}
	if (is_a_directory((*ast)->token->content[0]))
		return (exec_cmd_print_error(ast));
	if ((*ast)->token->token == CMD)
		return (exec_cmd_execve(ast, lists));
	return (1);
}

int	exec_cmd_execve_child(t_tree **ast, t_lists *lists)
{
	char	**strings_env;

	setup_child_signals();
	if (close_origin_fds(lists->origin_fds) == -1)
		malloc_error_freelists_exit(lists);
	strings_env = lst_to_array(lists->env);
	if (errno == ENOMEM)
		malloc_error_freelists_exit(lists);
	execve((*ast)->token->content[0], (*ast)->token->content, strings_env);
	perror("execve");
	free_array(strings_env);
	free_lists(lists);
	exit(-1);
}
