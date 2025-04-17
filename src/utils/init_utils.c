/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:04:05 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/16 01:53:37 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

void	free_list(t_var **l)
{
	t_var	*tmp;
	t_var	*next_node;

	tmp = *l;
	if (!*l)
		return ;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
	*l = NULL;
}

void	set_node(t_var *node, int mode)
{
	if (mode == 1)
	{
		node->env = 1;
		node->exported = 0;
	}
	if (mode == 2)
	{
		node->env = 0;
		node->exported = 1;
	}
	if (mode == 3)
	{
		node->env = 1;
		node->exported = 1;
	}
	else
	{
		node->env = 0;
		node->exported = 0;
	}
}

void	add_first_node(t_var **lst, t_var *new, char *s, int mode)
{
	char	**key_value;

	key_value = ft_split(s, '=');
	new->key = ft_strdup(key_value[0]);
	if (key_value[1])
		new->value = ft_strdup(key_value[1]);
	free_array(key_value);
	*lst = new;
	new->next = NULL;
	set_node(new, mode);
}

// 0 = aucun des deux / 1 = env / 2 = export / 3 = env + export
void	add_back_var(t_var **lst, char *s, int mode)
{
	t_var	*ptr;
	t_var	*new;
	char	**key_value;

	new = malloc(sizeof(t_var));
	if (new == NULL)
	{
		/* free_list(lst); */
		exit(139);
	}
	if (*lst == NULL)
		add_first_node(lst, new, s, mode);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->next = NULL;
		/* Value non init avant */
		new->value = NULL;
		key_value = ft_split(s, '=');
		new->key = ft_strdup(key_value[0]);
		if (key_value[1])
			new->value = ft_strdup(key_value[1]);
		free_array(key_value);
		set_node(new, mode);
	}
}
