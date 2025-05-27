/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fcts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:40:14 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:49:44 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "structs.h"
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

int	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}

void	free_lists(t_lists *lists)
{
	t_tree	*tree_to_free;

	if (!lists)
		return ;
	if (lists->env)
	{
		free_list(lists->env);
		free(lists->env);
	}
	if (lists->ast)
	{
		if (*lists->ast)
		{
			tree_to_free = *lists->ast;
			free_tree(&tree_to_free);
		}
		free(lists->ast);
	}
	if (lists->pipes)
		free(lists->pipes);
}
