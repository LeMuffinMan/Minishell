/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:03:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/23 04:15:28 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "tree.h"
#include <stdlib.h>

static void	cost_priority(t_token **node, bool flag)
{
	if (!flag)
	{
		if (((*node)->token == L_PARENTHESIS) || ((*node)->token == R_PARENTHESIS))
			(*node)->priority = 0;
		else if ((*node)->token == O_OR)
			(*node)->priority = 1;
		else if ((*node)->token == O_AND)
			(*node)->priority = 2;
	}
	else
	{
		if ((*node)->token == O_OR)
			(*node)->priority = 10;
		else if ((*node)->token == O_AND)
			(*node)->priority = 10;
		else if ((*node)->token == PIPE)
			(*node)->priority = 3;
		else if (((*node)->token == R_IN) || ((*node)->token == HD)
			|| ((*node)->token == APPEND) || ((*node)->token == TRUNC))
			(*node)->priority = 4;
		else if ((*node)->token == CMD || ((*node)->token == BUILT_IN))
			(*node)->priority = 5;
	}
}

void	assign_priority(t_token **head, bool flag)
{
	t_token	*tmp;

	if (!*head)
		return ;
	if ((*head)->prev)
	{
		while ((*head)->prev)
			(*head) = (*head)->prev;
	}
	tmp = *head;
	while (tmp->next)
	{
		cost_priority(&tmp, flag);
		tmp = tmp->next;
	}
	cost_priority(&tmp, flag);
}

void	set_bool_seq(t_tree **root)
{
	t_tree	*tmp;

	if (!*root)
		return ;
	tmp = *root;
	set_bool_seq(&tmp->left);
	set_bool_seq(&tmp->right);
	tmp->token->seq = false;
}

void	last_node(t_token **node, bool flag)
{
	if (!(*node))
		return ;
	if (flag == false)
	{
		while ((*node)->next)
			*node = (*node)->next;
	}
	else
	{
		while ((*node)->priority != 10)
			*node = (*node)->next;
		*node = (*node)->next;
	}
}

t_token	*find_best_priority(t_token *start, t_token *end, bool flag)
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
	if ((best->priority == 10 && !flag) || (best->seq == false))
		return (NULL);
	return (best);
}
