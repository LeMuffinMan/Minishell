/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:02:11 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:41:34 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "init.h"
#include "libft.h"
#include "malloc_error_handlers.h"
#include "structs.h"
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int	init_last_cmd_var(char *name, t_var **env)
{
	char	*s;
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strjoin(buf, name);
	else
		s = ft_strdup(name);
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

int	add_default_shell_vars(t_var **env)
{
	char	*s;

	s = ft_strdup("SHLVL=1");
	if (!s)
		return (malloc_free_string(s));
	if (add_back_var(env, s, 3) == -1)
		return (malloc_free_string(s));
	free(s);
	s = ft_strdup("OLDPWD=");
	if (!s)
		return (malloc_free_string(s));
	if (add_back_var(env, s, 1) == -1)
		return (malloc_free_string(s));
	free(s);
	return (0);
}

int	init(t_lists *lists, char **av, char **env)
{
	if (init_lists(lists) == -1)
	{
		return (-1);
	}
	if (init_env(lists->env, env, av[0]) == -1)
	{
		free_lists(lists);
		return (-1);
	}
	return (0);
}
