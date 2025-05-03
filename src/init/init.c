/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:02:11 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:06:46 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "env_utils.h"
#include "libft.h"
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

//a revoir 
int	init_last_cmd_var(char *name, t_var **env)
{
	char	*s;
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strjoin(buf, name);
	else
		s = ft_strdup(name);
	add_back_var(env, s, 1);
	free(s);
	return (0);
}

int	build_minimal_env(t_var **env, char *arg)
{
	char	*s;
	char	buf[PATH_MAX];

	s = NULL;
	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strdup(buf);
	else
	{
		perror("getcwd");
		/* (void)s = NULL; */
		//  1 ou errno ?
		//  free avant !
		return (1);
	}
	s = ft_strjoin("PWD=", s);
	add_back_var(env, s, 3);
	free(s);
	s = ft_strdup("SHLVL=1"); // si le SHLVL est negatif il met SHLVL = 0
	add_back_var(env, s, 3);
	free(s);
	s = ft_strdup("OLDPWD="); // updated in CD seulement ?
	add_back_var(env, s, 1);
	free(s);
	init_last_cmd_var(arg, env);
	add_back_var(env, "_=env", 0);
	return (0);
}

// a tester
// des char chelou avant le SHLVL
int	init_and_incremente_shlvl(char *s, t_var **env)
{
	int		n;
	int		i;
	char	*line;
	char	*shlvl_n;

	i = 7;
	line = NULL;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			break ;
		i++;
	}
	if ((size_t)i == ft_strlen(s))
	{
		n = ft_atoi(s + 6);
		if (n < 0)
			line = ft_strdup("SHLVL=0");
		else
		{
			shlvl_n = ft_itoa(n + 1);
			line = ft_strjoin("SHLVL=", shlvl_n);
			free(shlvl_n);
		}
	}
	if (!line)
		line = ft_strdup("SHLVL=1");
	add_back_var(env, line, 3);
	free(line);
	return (0);
}

//revoir la variable _
int	init_env(t_var **new_env, char **env, char *name_of_program)
{
	int	i;

	if (!*env)
		return (build_minimal_env(new_env, name_of_program));
	i = 0;
	while (env[i])
	{
		// des choses a revoir sur le shell level !
		// on doit gerer SHLVL+=1 ?
		if (!ft_strncmp("_=", env[i], 2))
			i++;
		if (!env[i])
			break ;
		if (!ft_strncmp("SHLVL=", env[i], 6))
			init_and_incremente_shlvl(env[i], new_env);
		else
			add_back_var(new_env, env[i], 3); // pas sur
		/* t_var *tmp; */
		/**/
		/* tmp = *new_env; */
		/* while(tmp) */
		/* 	tmp = tmp->next; */
		/* printf("%s=%s\n", tmp->key, tmp->value); */
		i++;
	}
	init_last_cmd_var(name_of_program, new_env);
	add_back_var(new_env, "?=0", 0);
	return (0);
}

