/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:56:37 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "exec.h"
#include "exec_boolops.h"
#include "libft.h"
#include <readline/readline.h>
#include <signals.h>
#include <stdio.h>
#include <unistd.h>

int	builtins(char **arg, t_lists *lists)
{
	if (!*arg)
		return (1);
	else if (!ft_strncmp(arg[0], "echo", 5))
		return (builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (builtin_cd(arg, lists->env));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (builtin_pwd(lists->env));
	else if (!ft_strncmp(arg[0], "export", 7))
		return (builtin_export(lists->env, arg));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (builtin_unset(lists->env, arg));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (builtin_env(lists->env, arg));
	else if (!ft_strncmp(arg[0], "exit", 5))
		return (builtin_exit(arg, lists->ast, lists));
	else
		return (1);
}

int	exec_error_cases(t_tree **ast)
{
	if ((*ast)->token->error == 127 || (*ast)->token->error == 126
		|| (*ast)->token->error == 21)
		return (error_cmd((*ast)->token->content[0], (*ast)->token->error));
	if ((*ast)->token->error == 5)
	{
		ft_putstr_fd((*ast)->token->content[0], 2);
		return (5);
	}
	if ((*ast)->token->token == D_QUOTE && (*ast)->token->error == 130)
		return (error_cmd("\"\"", 127));
	if ((*ast)->token->token == SPACE && (*ast)->token->error == 0)
		return (0);
	return (0);
}

int	exec_ast_boolops_cases(t_tree **ast, t_lists *lists)
{
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (exec_boolop(ast, lists));
	if ((*ast)->token->token == GROUP_PARENTHESIS)
		return (exec_parenthesis(ast, lists));
	if ((*ast)->token->token == GROUP_BOOLOP)
		return (exec_group_boolop(ast, lists));
	return (1);
}

int	exec_ast(t_tree **ast, t_lists *lists)
{
	struct sigaction	sa_ignore;
	struct sigaction	sa_orig;

	if (!*ast)
		return (lists->exit_code);
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR
		|| (*ast)->token->token == GROUP_PARENTHESIS
		|| (*ast)->token->token == GROUP_BOOLOP)
		return (exec_ast_boolops_cases(ast, lists));
	if ((*ast)->token->error == 2)
	{
		ft_putendl_fd((*ast)->token->content[0], 2);
		return ((*ast)->token->error);
	}
	if (((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
			|| (*ast)->token->token == TRUNC) || ((*ast)->token->token == PIPE)
		|| ((*ast)->token->token == HD) || ((*ast)->token->error != 126
			&& ((*ast)->token->token == BUILT_IN
				|| (*ast)->token->token == CMD)))
		return (exec_group_cmd(ast, lists));
	return (exec_error_cases(ast));
}
