/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_boolops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:35:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:37:36 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec_boolops.h"
#include "libft.h"
#include "malloc_error_handlers.h"
#include "signals.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int	exec_boolop(t_tree **ast, t_lists *lists)
{
	int	exit_code;

	exit_code = 0;
	if ((*ast)->left)
	{
		exit_code = exec_ast(&((*ast)->left), lists);
		if (errno == ENOMEM)
			return (errno);
		if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0
				&& (*ast)->token->token == O_OR))
		{
			exit_code = exec_ast(&((*ast)->right), lists);
			if (errno == ENOMEM)
				return (errno);
			return (exit_code);
		}
		else
			return (exit_code);
	}
	return (1);
}

int	exec_parenthesis_child(t_tree **ast, t_lists *lists)
{
	t_tree	*sub_ast;
	int		exit_code;

	exit_code = 0;
	if (close_origin_fds(lists->origin_fds) == -1)
		malloc_error_parenthesis_child(lists, NULL);
	setup_child_signals();
	sub_ast = parse((*ast)->token->group->content[0], *lists->env, lists);
	if (errno == ENOMEM)
		malloc_error_parenthesis_child(lists, NULL);
	exit_code = exec_ast(&sub_ast, lists);
	if (errno == ENOMEM)
		malloc_error_parenthesis_child(lists, &sub_ast);
	free_tree(&sub_ast);
	free_lists(lists);
	exit(exit_code);
}

int	exec_parenthesis(t_tree **ast, t_lists *lists)
{
	pid_t				pid;
	struct sigaction	sa_ignore;
	struct sigaction	sa_orig;

	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	pid = fork();
	if (pid < 0)
		return (ENOMEM);
	if (pid == 0)
	{
		exec_parenthesis_child(ast, lists);
	}
	else
	{
		lists->exit_code = wait_children(pid, pid);
		sigaction(SIGINT, &sa_orig, NULL);
		return (lists->exit_code);
	}
	return (1);
}

int	exec_group_cmd(t_tree **ast, t_lists *lists)
{
	if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC || (*ast)->token->token == HD)
		return (redirect_stdio(ast, lists));
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, lists));
	if ((*ast)->token->error != 126 && ((*ast)->token->token == BUILT_IN
			|| (*ast)->token->token == CMD
			|| !ft_strncmp((*ast)->token->content[0], "source", 7)))
	{
		lists->exit_code = exec_cmd(ast, lists);
		return (lists->exit_code);
	}
	return (-1);
}

int	exec_group_boolop(t_tree **ast, t_lists *lists)
{
	t_tree	*sub_ast;
	int saved_errno;

	sub_ast = NULL;
	sub_ast = parse((*ast)->token->content[0], *lists->env, lists);
	if (errno == ENOMEM)
		return (errno);
	lists->exit_code = exec_ast(&sub_ast, lists);
	if (errno == ENOMEM)
	{
		saved_errno = errno;
		free_tree(&sub_ast);
		errno = saved_errno;
		return (errno);
	}
	free_tree(&sub_ast);
	return (lists->exit_code);
}
