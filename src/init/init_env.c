/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:29:03 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:33:27 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "init.h"
#include "libft.h"

static int	init_env_shlvl(t_var **new_env, char *env_line)
{
	if (!ft_strncmp("SHLVL=", env_line, 6))
	{
		if (init_and_incremente_shlvl(env_line, new_env) == -1)
			return (-1);
	}
	return (0);
}

static int	init_env_pwd(t_var **new_env, char *env_line)
{
	if (!ft_strncmp("PWD=", env_line, 5))
	{
		if (init_pwd(new_env) == -1)
			return (-1);
	}
	return (0);
}

static int	process_env_line(t_var **new_env, char *env_line)
{
	if (!ft_strncmp("_=", env_line, 2))
		return (0);
	if (init_env_shlvl(new_env, env_line) == -1)
		return (-1);
	if (init_env_pwd(new_env, env_line) == -1)
		return (-1);
	if (ft_strncmp("SHLVL=", env_line, 6) && ft_strncmp("PWD=", env_line, 5))
	{
		if (add_back_var(new_env, env_line, 3) == -1)
			return (-1);
	}
	return (0);
}

static int	process_env_vars(t_var **new_env, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (process_env_line(new_env, env[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	init_env(t_var **new_env, char **env, char *program_name)
{
	if (!env || !*env)
		return (build_minimal_env(new_env, program_name));
	if (process_env_vars(new_env, env) == -1)
		return (-1);
	return (init_last_cmd_var(program_name, new_env) == -1);
}
