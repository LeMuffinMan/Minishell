/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:12:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/16 01:26:22 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "list.h"
#include "stdlib.h"

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
	new_node->token = token;
	new_node->priority = 10;
	return (new_node);
}

static t_tree	*parse_list(t_token *start, t_token *end)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*arg;

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
	free_parse(head->token, NULL, 0);
}
