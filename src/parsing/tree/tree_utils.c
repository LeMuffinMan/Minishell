/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:03:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/13 20:11:24 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "tree.h"
#include <stdlib.h>

static void	cost_priority(t_token **node, bool flag)
{
	if (!flag)
	{
		if ((*node)->token == GROUP_PARENTHESIS)
			(*node)->priority = PRIO_PARENTHESIS;
		else if ((*node)->token == O_OR)
			(*node)->priority = PRIO_O_OR;
		else if ((*node)->token == O_AND)
			(*node)->priority = PRIO_O_AND;
	}
	else
	{
		if ((*node)->token == GROUP_PARENTHESIS)
			(*node)->priority = PRIO_IGNORE;
		else if ((*node)->token == PIPE)
			(*node)->priority = PRIO_PIPE;
		else if (((*node)->token == R_IN) || ((*node)->token == HD)
			|| ((*node)->token == APPEND) || ((*node)->token == TRUNC))
			(*node)->priority = PRIO_REDIR;
		else if ((*node)->token == CMD || ((*node)->token == BUILT_IN))
			(*node)->priority = PRIO_CMD;
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
	if (!(*node) || !(*node)->next)
		return ;
	if (flag == false)
	{
		while ((*node)->next)
			*node = (*node)->next;
	}
	else
	{
		while ((*node)->next)
		{
			if ((*node)->token == O_AND || (*node)->token == O_OR)
				break ;
			*node = (*node)->next;
		}
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
