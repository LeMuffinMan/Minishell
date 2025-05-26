/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:33:13 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:02:10 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"

int	env_size(t_var *env)
{
	int		i;
	t_var	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	free_node(t_var **node)
{
	free((*node)->key);
	free((*node)->value);
	free(*node);
	return (0);
}

int	free_single_node(t_var **node, t_var **env)
{
	(void)env;
	free_node(node);
	env = NULL;
	return (0);
}

int	delete_node(t_var **node, t_var **env)
{
	t_var	*tmp;
	t_var	*last;

	last = NULL;
	if (env_size(*env) == 1)
		return (free_single_node(node, env));
	tmp = *env;
	while (tmp)
	{
		if (tmp == *node)
		{
			if (!last)
				*env = tmp->next;
			else
				last->next = tmp->next;
			return (free_node(node));
		}
		last = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	builtin_unset(t_var **env, char **arg)
{
	t_var	*node_to_remove;
	int		i;

	i = 1;
	node_to_remove = NULL;
	while (arg[i])
	{
		node_to_remove = is_known_key(env, arg[i]);
		if (node_to_remove)
			delete_node(&node_to_remove, env);
		i++;
	}
	return (0);
}
