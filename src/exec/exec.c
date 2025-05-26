/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/23 21:54:57 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"
#include "shell_fct.h"
#include "exec_boolops.h"
#include "builtins.h"
#include "exec.h"
#include "utils.h"
#include "env_utils.h"
#include "libft.h"
#include <unistd.h>
#include <signals.h>
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline

//ULTRABONUS
int builtins_ultra_bonus(char **arg, t_lists *lists)
{
	if (!ft_strncmp(arg[0], "source", 7)) // mieux proteger ULTRA BONUS
		return (builtin_source((*lists->ast)->right->token->content[0], lists->env));
	/* else if (!ft_strncmp(arg[0], "history", 7)) // ULTRA BONUS */
	/* 	return (builtin_history((*ast)->right->token->content[0], env)); */
	return (1);
}

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
		return (builtins_ultra_bonus(arg, lists));
}

int exec_error_cases(t_tree **ast)
{
	if ((*ast)->token->error == 127 || (*ast)->token->error == 126
		|| (*ast)->token->error == 21)
		return (error_cmd((*ast)->token->content[0], (*ast)->token->error));
	if ((*ast)->token->error == 5)
	{
		ft_putstr_fd((*ast)->token->content[0], 2);
		return (5);
	}
	//c'est un syntax error ?
	return (-1); //quelle code d'erreur si on est entre dans aucun case ?
}

int	exec_ast(t_tree **ast, t_lists *lists)
{
	t_alias		*alias;
	t_shell_fct	*shell_fct;

	if (!*ast)
		return (127); // on devrait peut etre reagir dans le main pour ca
	//signaux a virer ?
	/* if ((*ast)->token->token == GROUP_PARENTHESIS) */
	/* 	dprintf(2, "(*ast) token = GROUP_PARENTHESIS\n"); */
	/* if ((*ast)->token->token == GROUP_BOOLOP) */
	/* 	dprintf(2, "(*ast) token = GROUP_BOOLOP\n"); */
	/* if ((*ast)->token->token == GROUP_PARENTHESIS) */
	/* 	dprintf(2, "(*ast) token = GROUP_PARENTHESIS\n"); */
	/* if ((*ast)->token->token == GROUP_BOOLOP) */
	/* 	dprintf(2, "(*ast) token = GROUP_BOOLOP\n"); */
	struct sigaction sa_ignore, sa_orig;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	//signaux a virer ?
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (exec_boolop(ast, lists));
	if ((*ast)->token->token == GROUP_PARENTHESIS)
		return (exec_parenthesis(ast, lists));
	if ((*ast)->token->token == GROUP_BOOLOP)
		return (exec_group_boolop(ast, lists));	
	//l'error code 2 correspond a quoi deja ?
	if ((*ast)->token->error == 2)
	{
		ft_putendl_fd((*ast)->token->content[0], 2);
		return ((*ast)->token->error);
	}
	if (((*ast)->token->token == R_IN || (*ast)->token->token == APPEND || (*ast)->token->token == TRUNC) || 
		((*ast)->token->token == PIPE) || ((*ast)->token->token == HD) ||
		((*ast)->token->error != 126 && ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)))
		return (exec_group_cmd(ast, lists));
	//
	// un token Alias
	alias = is_a_known_alias((*ast)->token->content[0], lists->aliases);
	if ((*ast)->token->error == 127 && alias)
		return (exec_alias(ast, lists, alias));
	// un token shell_func
	shell_fct = is_a_known_shell_fct((*ast)->token->content[0],
			lists->shell_fcts);
	if (shell_fct)
	if ((*ast)->token->error == 127 && shell_fct)
		return (exec_shell_fct(ast, lists, shell_fct));
	return (exec_error_cases(ast));
}

