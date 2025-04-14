/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:12:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/14 14:48:50 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "list.h"
#include "stdlib.h"
#include <stdio.h>

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


static void	assign_priority(t_token **head)
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

static void	last_node(t_token **node)
{
	if (!(*node))
		return ;
	while((*node)->next)
		*node = (*node)->next;
	}
	
static t_tree	*add_new_node(t_token *token, t_tree *left, t_tree *right)
{
	t_tree	*new_node;
	
	if (!token)
	return (NULL);
	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	if (left)
		new_node->left = left;
	if (right)
		new_node->right = right;
	new_node->content = token;
	new_node->priority = 10;
	return (new_node);
}

static t_token *find_best_priority(t_token *start, t_token *end)
{
    t_token *best;
    t_token *current;

    if (!start || !end)
		return (NULL);
    best = start;
    current = start;
    while (current != end->next)
    {
		// if (!current || !best) {
        //     printf("Error: Null pointer encountered in find_best_priority\n");
        //     return (NULL);
        // }
		if (current->priority < best->priority)
		{
			printf("Current token: %s, Priority: %d\n", current->content[0], current->priority);
			best = current;
		}
        current = current->next;
    }
    return (best);
}

static t_tree *parse_list(t_token *start, t_token *end)
{
	t_tree *left;
	t_tree *right;
	t_token *arg;

	if (!start || !end)
		return (add_new_node(start, NULL, NULL));
	arg = find_best_priority(start, end);
	if (!arg)
		return (NULL);
	left = NULL;
	right = NULL;
	if (arg->prev && arg != start)
		left = parse_list(start, arg->prev);
	if (arg->next && arg != end)
		right = parse_list(arg->next, end);
	return (add_new_node(arg, left, right));
}

void	add_to_root(t_token *node, t_tree **root)
{
	t_token	*end;
	
	assign_priority(&node);
	end = node;
	last_node(&end);
	*root = parse_list(node, end);	
}

void	free_tree(t_tree *head)
{
	if (!head)
		return ;
	free_tree(head->left);
	free_tree(head->right);
	free(head);
}
