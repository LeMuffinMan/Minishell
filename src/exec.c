/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/03 17:57:58 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	exec(char **arg, t_env **env)
{
	/* faire un switch case et y mettre les returns */
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 4))
		return(builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 2))
		return(builtin_cd(arg));
	else if (!ft_strncmp(arg[0], "pwd", 3))
		return(builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 6))
		return (builtin_export(env, arg));
	else if (!ft_strncmp(arg[0], "unset", 5))
		return(builtin_unset(env, arg));
	else if (!ft_strncmp(arg[0], "env", 3))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 4))
		return(builtin_exit(arg, env));
	else // pas printf ! utiliser fd_putstr_fd
		printf("error : not a builtin\n");
	return (0);
}
