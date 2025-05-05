/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:27:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:22:26 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <stdlib.h>

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

int copy_node(t_var **new_node, t_var **tmp)
{
	(*new_node)->key = ft_strdup((*tmp)->key);
	(*new_node)->value = ft_strdup((*tmp)->value);
	(*new_node)->exported = (*tmp)->exported;
	(*new_node)->env = (*tmp)->env;
	(*new_node)->alias = (*tmp)->alias;
	(*new_node)->shell_fct = (*tmp)->shell_fct;
	(*new_node)->next = NULL;
	return (0);
}

t_var	*copy_list(t_var **env)
{
	t_var	*tmp;
	t_var	*last;
	t_var	*new_node;
	t_var	*copy;

	tmp = *env;
	last = NULL;
	copy = NULL;
	while (tmp)
	{
		new_node = malloc(sizeof(t_var));
		if (!new_node)
			return (NULL);
		copy_node(&new_node, &tmp);
		if (last)
			last->next = new_node;
		else
			copy = new_node;
		last = new_node;
		tmp = tmp->next;
	}
	return (copy);
}

int	array_size(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}
