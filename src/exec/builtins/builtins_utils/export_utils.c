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

int	is_valid_identifier(char *var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	j = 0;
	while (j < i)
	{
		if (var[j] == '+' || var[j] == '-' || var[j] == '/' || var[j] == '*')
		{
			if (!(j == i - 1 && var[j] == '+'))
				return (1);
		}
		j++;
	}
	return (0);
}

char	**concat_var(char **arg)
{
	char	**key_value;
	char	*tmp;
	int		i;

	key_value = malloc(sizeof(char *) * 3);
	if (!key_value)
		return (NULL);
	key_value[0] = ft_strdup(arg[0]);
	key_value[1] = ft_strdup("");
	if (!arg[0])
		return (key_value);
	i = 1;
	while (arg[i])
	{
		tmp = key_value[1];
		key_value[1] = ft_strjoin(key_value[1], arg[i]);
		free(tmp);
		/* if (arg[i + 1]) */
		/* { */
		/* 	tmp = key_value[1]; */
		/* 	key_value[1] = ft_strjoin(key_value[1], "="); */
		/* 	free(tmp); */
		/* } */
		i++;
	}
	key_value[2] = NULL;
	free_array(arg);
	return (key_value);
}

int	is_increment_operator(char *s)
{
	if (s[ft_strlen(s) - 1] == '+')
		return (1);
	else
		return (0);
}

char	*trim_operator(char *s)
{
	char	*trimmed;

	trimmed = NULL;
	trimmed = malloc(sizeof(char) * ft_strlen(s));
	ft_strlcpy(trimmed, s, ft_strlen(s));
	free(s);
	return (trimmed);
}

int	add_new_var(t_var **env, char **key_value)
{
	char	*s;
	char	*tmp;

	// si ce n'est pas une variable protegee !
	s = ft_strjoin(key_value[0], "=");
	tmp = s;
	s = ft_strjoin(s, key_value[1]);
	free(tmp);

	add_back_var(env, s, 3);
	free(s);
	return (0);
}
