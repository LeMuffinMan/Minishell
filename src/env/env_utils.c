/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:15:02 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/30 23:13:46 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include <errno.h>

t_var	*is_known_key(t_var **env, char *key)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_value(t_var **env, char *key)
{
	char	*value;
	t_var	*tmp;

	if (!env || !*env)
		return (NULL);
	tmp = *env;
	value = NULL;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1))
		{
			value = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	return (value);
}

t_var *is_known_exported_key(t_var **env, char *key)
{
	t_var *tmp;

	tmp = *env;
	while(tmp)
	{
		if ((!ft_strncmp(tmp->key, key, ft_strlen(tmp->key) + 1) && tmp->exported == 1) ||
			  (!ft_strncmp(tmp->key, "PS1", ft_strlen(tmp->key) + 1) && !ft_strncmp(key, tmp->key, ft_strlen(tmp->key)))) // A VIRERRRRR !!!!!!!!
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

