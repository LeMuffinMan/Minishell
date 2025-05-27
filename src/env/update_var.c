/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:39:16 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:39:19 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "structs.h"
#include <errno.h>
#include <stdlib.h>

int	update_last_cmd_var(t_var **env, char *last_cmd)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp("_", tmp->key, 2))
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
		add_back_var(env, last_cmd, 1);
	else
	{
		free(tmp->value);
		tmp->value = ft_strdup(last_cmd);
	}
	return (0);
}

int	update_last_arg_var(t_var **env, char **content)
{
	t_var	*tmp;
	int		i;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "_", 2) && tmp->env == 0)
		{
			i = 0;
			while (content[i])
				i++;
			if (tmp->value)
				free(tmp->value);
			tmp->value = ft_strdup(content[i]);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	update_existing_value(t_var *node, char *var, int inc)
{
	char	*value;
	char	*tmp;

	value = ft_strchr(var, '=');
	if (!value)
		return (0);
	if (inc)
	{
		tmp = node->value;
		node->value = ft_strjoin(node->value, value + 1);
		if (!node->value)
			return (malloc_free_string(tmp));
		free(tmp);
	}
	else
	{
		free(node->value);
		node->value = ft_strdup(value + 1);
		if (!node->value)
			return (errno);
	}
	return (0);
}

t_var	*update_if_known_key(t_var **env, char *var, char *s, int inc)
{
	t_var	*node;
	char	**key;

	key = ft_split(s, '=');
	if (!key)
		return (NULL);
	node = is_known_exported_key(env, key[0]);
	free_array(key);
	if (node)
	{
		if (update_existing_value(node, var, inc) == -1)
			return (NULL);
	}
	return (node);
}
