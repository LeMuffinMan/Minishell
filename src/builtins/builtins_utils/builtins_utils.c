/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:27:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:00:23 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <errno.h>

int	swap_nodes(t_var *n1, t_var *n2)
{
	char	*tmp_key;
	char	*tmp_value;
	int		tmp_env;
	int		tmp_exported;

	tmp_key = n1->key;
	tmp_value = n1->value;
	tmp_env = n1->env;
	tmp_exported = n1->exported;
	n1->key = n2->key;
	n1->value = n2->value;
	n1->env = n2->env;
	n1->exported = n2->exported;
	n2->key = tmp_key;
	n2->value = tmp_value;
	n2->env = tmp_env;
	n2->exported = tmp_exported;
	return (0);
}

int	copy_node(t_var **new_node, t_var **tmp)
{
	(*new_node)->key = ft_strdup((*tmp)->key);
	if (errno == ENOMEM)
		return (errno);
	(*new_node)->value = ft_strdup((*tmp)->value);
	if (errno == ENOMEM)
		return (errno);
	(*new_node)->exported = (*tmp)->exported;
	(*new_node)->env = (*tmp)->env;
	(*new_node)->next = NULL;
	return (0);
}

static t_var	*create_and_link_node(t_var *tmp, t_var **last, t_var **copy)
{
	t_var	*new_node;

	new_node = malloc(sizeof(t_var));
	if (!new_node)
		return (NULL);
	copy_node(&new_node, &tmp);
	if (errno == ENOMEM)
	{
		free(new_node);
		return (NULL);
	}
	if (*last)
		(*last)->next = new_node;
	else
		*copy = new_node;
	*last = new_node;
	return (new_node);
}

t_var	*copy_list(t_var **env)
{
	t_var	*tmp;
	t_var	*last;
	t_var	*copy;

	tmp = *env;
	last = NULL;
	copy = NULL;
	while (tmp)
	{
		if (!create_and_link_node(tmp, &last, &copy))
			return (NULL);
		tmp = tmp->next;
	}
	return (copy);
}
