/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_back_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:36:00 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:36:02 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include "libft.h"
#include "structs.h"
#include <errno.h>
#include <stdlib.h>

int	free_node_var(t_var *node, char **array)
{
	if (array)
		free_array(array);
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (-1);
}

static int	new_node_allocation(t_var **new)
{
	*new = malloc(sizeof(t_var));
	if (*new == NULL)
		return (-1);
	(*new)->next = NULL;
	(*new)->value = NULL;
	(*new)->key = NULL;
	return (0);
}

static int	process_key_value(t_var *new, char *s)
{
	char	**key_value;

	key_value = ft_split_on_first_equal(s);
	if (errno == ENOMEM)
		return (free_node_var(new, NULL));
	if (!new->key)
	{
		new->key = ft_strdup(key_value[0]);
		if (!new->key)
			return (free_node_var(new, key_value));
	}
	if (key_value && key_value[1])
	{
		if (!new->value)
		{
			new->value = ft_strdup(key_value[1]);
			if (!new->value)
				return (free_node_var(new, key_value));
		}
	}
	free_array(key_value);
	return (0);
}

static int	add_node_to_list(t_var **lst, t_var *new, char *s, int mode)
{
	t_var	*ptr;

	if (!lst || *lst == NULL)
		return (add_first_node(lst, &new, s, mode));
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	return (0);
}

int	add_back_var(t_var **lst, char *s, int mode)
{
	t_var	*new;
	int		exit_code;

	if (new_node_allocation(&new) == -1)
		return (-1);
	exit_code = add_node_to_list(lst, new, s, mode);
	if (exit_code != 0)
		return (exit_code);
	exit_code = process_key_value(new, s);
	if (exit_code != 0)
		return (exit_code);
	return (set_node(&new, mode));
}
