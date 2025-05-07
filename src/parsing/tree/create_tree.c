/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:12:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/07 13:37:40 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "list.h"
#include <stdlib.h>
#include "libft.h"

static void	assign_head(t_tree **root, t_tree *head)
{
	t_tree	*tmp;

	tmp = *root;
	tmp->head = head;
	if (!tmp)
		return ;
	if (tmp->left)
		assign_head(&(tmp->left), head);
	if (tmp->right)
		assign_head(&(tmp->right), head);
}

static t_tree	*add_new_node(t_token *token, t_tree *left,
								t_tree *right)
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

static t_tree	*parse_list(t_token *start, t_token *end,
							bool flag)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*arg;

	if (!start || !end)
		return (add_new_node(start, NULL, NULL));
	arg = find_best_priority(start, end, flag);
	if (!arg)
		return (NULL);
	left = NULL;
	right = NULL;
	if (arg->prev && arg != start)
		left = parse_list(start, arg->prev, flag);
	if (arg->next && arg != end)
		right = parse_list(arg->next, end, flag);
	return (add_new_node(arg, left, right));
}

void	add_to_root(t_token *node, t_tree **root, bool flag)
{
	t_token	*end;
	t_token	*start;

	end = node;
	start = node;
	assign_priority(&node, flag);
	last_node(&end, flag);
	if (flag && start->prev)
		start->priority = 0;
	*root = parse_list(node, end, flag);
	if (flag)
		set_bool_seq(root);
	assign_head(root, *root);
}

void	free_tree(t_tree **head)
{
	t_tree	*left;
	t_tree	*right;
	t_token	*tmp_token;
	t_tree	*tmp_head;

	tmp_head = *head;
	left = (*head)->left;
	right = (*head)->right;
	tmp_token = (*head)->token;
	if (!*head || !head)
		return ;
	if (left)
		left = (*head)->left;
	if (right)
		right = (*head)->right;
	if (left)
		free_tree(&left);
	if (right)
		free_tree(&right);
	if (tmp_token)
	{
		if (tmp_token->content)
			free_tab(tmp_token->content);
		free(tmp_token);
		tmp_token = NULL;
	}
	if (tmp_head)
		free(tmp_head);
}
