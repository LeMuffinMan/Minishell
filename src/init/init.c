/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:02:11 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 16:06:27 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "init_utils.h"
#include "malloc_error_handlers.h" //pour le malloc error 
#include "libft.h"
#include "structs.h"
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

// a revoir
// a deplacer dans un init_shell_var ?
int	init_last_cmd_var(char *name, t_var **env)
{
	char	*s;
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strjoin(buf, name);
	else
	{
		// gerer l'erreur d'un getcwd qui a echoue
		s = ft_strdup(name);
	}
	if (!s)
		return (-1);
	if (add_back_var(env, s, 1) == -1)
	{
		free(s);
		return (-1);
	}
	free(s);
	return (0);
}


//Add_shell_Vars
int	add_default_shell_vars(t_var **env)
{
	char	*s;

	s = ft_strdup("SHLVL=1"); // si le SHLVL est negatif il met SHLVL = 0
	if (!s)
		return (malloc_free_string(s));
	if (add_back_var(env, s, 3) == -1)
		return (malloc_free_string(s));
	free(s);
	s = ft_strdup("OLDPWD="); // updated in CD seulement ?
	if (!s)
		return (malloc_free_string(s));
	if (add_back_var(env, s, 1) == -1)
		return (malloc_free_string(s));
	free(s);
	return (0);
}

int	build_minimal_env(t_var **env, char *arg)
{
	char	*s;

	s = NULL;
	if (get_cwd_init(s) == -1)
		return (-1);
	if (!s)
		return (-1);
	s = ft_strjoin("PWD=", s);
	if (!s)
		return (malloc_free_string(s));
	if (add_back_var(env, s, 3) == -1)
		return (malloc_free_string(s));
	free(s);
	if (add_default_shell_vars(env) == -1)
		return (-1);
	if (init_last_cmd_var(arg, env) == -1)
		return (-1);
	return (add_back_var(env, "_=env", 0));
}

// revoir la variable _
// 25 lignes qund on aura fix l'exit code et la var _
int	init_env(t_var **new_env, char **env, char *program_name)
{
	int	i;

	if (! env && !*env)
		return (build_minimal_env(new_env, program_name));
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp("_=", env[i], 2))
			i++;
		if (!env[i])
			break ;
		if (!ft_strncmp("SHLVL=", env[i], 6))
		{
			if (init_and_incremente_shlvl(env[i], new_env) == -1)
				return (-1);
		}
		else
		{
			if (add_back_var(new_env, env[i], 3) == -1) // pas sur
				return (-1);
		}
		i++;
	}
	if (init_last_cmd_var(program_name, new_env) == -1)
		return (-1);
	return (add_back_var(new_env, "?=0", 0)); // revoir l'init de l'exit code 
}
