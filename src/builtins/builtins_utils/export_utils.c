/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:22:41 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:22:42 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "utils.h"
#include <stdlib.h>
#include <errno.h>

int export_without_argument(t_var **env)
{
	t_var *copy;

	copy = NULL;
	copy = copy_list(env);
	if (!copy)
		return (errno);
	sort_list(&copy);
	if (errno == ENOMEM)
		return (errno);
	print_sorted_env(&copy);
	free_list(&copy);
	return (0);
}

int is_var_declaration(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '=' || ft_isdigit(arg[i]) || !ft_isalnum(arg[i]))
		return (0);
	while (arg[i])
	{
		if (arg[i] == '+' && arg[i + 1] && arg[i + 1] != '=') // += pas valide !
			return (0);
		if (!ft_isalnum(arg[i]))
		{
			if ((arg[i] == '=') || (arg[i + 1] && arg[i] == '+' && arg[i + 1] == '='))
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

char *get_full_variable_declaration(char **arg, int i, char **s)
{
	if (arg[i][ft_strlen(arg[i]) - 1] == '=') 
	{
		if (arg[i + 1] && !is_var_declaration(arg[i + 1])) 
		{
			*s = ft_strjoin(arg[i], arg[i + 1]);
			if (!*s)
				return (NULL);
		}
		else
		{
			*s = ft_strdup(arg[i]); 
			if (!*s)
				return (NULL);
		}
	}
	else 
	{
		*s = ft_strdup(arg[i]);
		if (!*s)
			return (NULL);
	}
	return (*s);
}


int is_var_exportation(char *s)
{
	int i;

	i = 0;
	if (s[i] == '=' || ft_isdigit(s[i]) || !ft_isalnum(s[i]))
		return (0);
	while(s[i + 1])
	{
		if (s[i] == '+' && s[i + 1] && s[i + 1] != '=')
			return (0);
		if ((s[i] < '0' || s[i] > 'z'))
			return (0);
		i++;
	}
	if (s[i] == '=')
		return (0);
	return (1);
}
