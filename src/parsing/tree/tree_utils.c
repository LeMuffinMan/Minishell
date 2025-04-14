/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:03:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/14 16:20:32 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "tree.h"
#include <stdlib.h>

static void	cost_priority(t_token **node)
{
	if ((*node)->token == O_OR)
		(*node)->priority = 1;
	else if ((*node)->token == O_AND)
		(*node)->priority = 2;
	else if ((*node)->token == PIPE)
		(*node)->priority = 3;
	else if (((*node)->token == R_IN) || ((*node)->token == HD)
		|| ((*node)->token == APPEND) || ((*node)->token == TRUNC))
		(*node)->priority = 4;
	else
		(*node)->priority = 5;
}

void	assign_priority(t_token **head)
{
	t_token	*tmp;

	if (!*head)
		return ;
	tmp = *head;
	while (tmp->next)
	{
		cost_priority(&tmp);
		tmp = tmp->next;
	}
	cost_priority(&tmp);
}

void	last_node(t_token **node)
{
	if (!(*node))
		return ;
	while ((*node)->next)
		*node = (*node)->next;
}

t_token	*find_best_priority(t_token *start, t_token *end)
{
	t_token	*best;
	t_token	*current;

	if (!start || !end)
		return (NULL);
	best = start;
	current = start;
	while (current != end->next)
	{
		if (current->priority < best->priority)
			best = current;
		current = current->next;
	}
	return (best);
}
