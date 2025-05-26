/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:36:27 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/16 17:56:24 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env_utils.h"
#include "libft.h"
#include "structs.h"
#include "limits.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "cd_utils.h"

int builtins_cd_oldpwd_path(t_var **env, char **path)
{
	if (!get_value(env, "OLDPWD"))
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	else
	{
		*path = ft_strdup(get_value(env, "OLDPWD"));
		if (!*path)
			return (errno);
	}
	return (0);
}

int try_change_directory(char *path, t_var **env, char **arg)
{
	if (!change_directory(path))
	{
		update_env(env);
		if (errno == ENOMEM)
			return (errno);
		if (!ft_strncmp(arg[1], "-", 2))
		{
			builtin_pwd(env);
			if (errno == ENOMEM)
				return (errno);
		}
		return (0);
	}
	return (1);
}

int handle_oldpwd(t_var **env, char **path)
{
	int exit_code;
	int res;

	exit_code = builtins_cd_oldpwd_path(env, path);
	if (exit_code != 0)
		return (exit_code);
	res = try_change_directory(*path, env, (char*[]){"cd", "-", NULL});
	free(*path);
	return (res);
}

int builtin_cd_with_arg(char **arg, t_var **env)
{
	char *path = NULL;
	int exit_code;

	if (!ft_strncmp(arg[1], "-", 2))
		return (handle_oldpwd(env, &path));
	if (!ft_strncmp(arg[1], "..", 3) && access("..", X_OK) != 0)
	{
		if (trim_pwd(&path) == -1)
			return (-1);
	}
	else
	{
		path = ft_strdup(arg[1]);
		if (!path)
			return (-1);
	}
	exit_code = try_change_directory(path, env, arg);
	free(path);
	return (exit_code);
}

int	builtin_cd(char **arg, t_var **env)
{
	if (array_size(arg) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (array_size(arg) == 1)
		return(builtin_cd_without_arg(env));
	if (array_size(arg) == 2)
		return (builtin_cd_with_arg(arg, env));
	return (0);
}
