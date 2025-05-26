/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:20:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:22:13 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "limits.h"
#include "structs.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int	change_directory(char *path)
{
	if (!path)
		return (0);
	if (chdir(path) == 0)
		return (0);
	else
	{
		perror(path);
		return (1);
	}
}

int	builtin_cd_without_arg(t_var **env)
{
	char	*s;

	s = get_value(env, "HOME");
	if (!s)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 1);
		return (1);
	}
	else
	{
		if (!change_directory(s))
		{
			update_env(env);
			if (errno == ENOMEM)
				return (errno);
		}
	}
	return (0);
}

int	find_last_slash(char *buf)
{
	int	i;

	i = ft_strlen(buf);
	while (i >= 0)
	{
		if (buf[i] == '/')
			break ;
		i--;
	}
	return (i);
}

int	trim_pwd(char **s)
{
	int		end;
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		end = find_last_slash(buf);
		*s = ft_strndup(buf, end);
		if (!*s)
			return (-1);
		return (0);
	}
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: \
			cannot access parent directories: No such file or directory", 2);
		return (1);
	}
}
