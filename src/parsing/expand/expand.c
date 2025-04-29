/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/29 20:09:17 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include "exec.h"

static void	expand_content_node(t_token **node, t_var *list_env, int i, int j)
{
	char	**res;
	t_var	*value;

	res = NULL;
	value = get_value(&list_env, &(*node)->content[i][j]);
	
}

static bool	to_expand(t_token **node, t_var *list_env)
{
	int	i;
	int	j;
	
	i = 0;
	while ((*node)->content[i])
	{
		j = 0;
		while ((*node)->content[i][j])
		{
			if ((*node)->content[i][j] == '$' && ((*node)->content[i][j - 1] && (*node)->content[i][j - 1] != '\\'))
				expand_content_node(node, list_env, i, j);
			j++;
		}
		i++;
	}
	return (false);
}

void	init_expand(t_token **head, t_var *list_env)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (tmp->error != 0 || tmp->token == D_QUOTE)
			to_expand(&tmp, list_env);
		tmp = tmp->next;
	}
}
