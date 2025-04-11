/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:18:17 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// a tester
/* if (arg[0]) */
/* 	update_last_cmd_var(env, arg[0]); */
/* faire un switch case et y mettre les returns */
// 1 : ouvrir et remplir tous les here_docs
// 2 : open et rediriger tous les outputs / inputs vers / pour une cmd
// 3 : checker si c'est un builtin ou une cmd
// parent pour les builtins
// enfants pour les cmds ?
// 4 Wait
// 5 Mettre a jour l'environnement
int	exec(char **arg, t_var **env)
{
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 5))
		return (builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (builtin_cd(arg, env));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 7))
		return (builtin_export(env, arg));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (builtin_unset(env, arg));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 5))
		return (builtin_exit(arg, env));
	else
		ft_putstr_fd("child process will run any other cmd\n", 1);
	return (0);
}
