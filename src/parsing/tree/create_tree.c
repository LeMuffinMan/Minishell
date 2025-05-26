/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:12:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 01:14:57 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "list.h"
#include <stdlib.h>
#include "libft.h"
#include <errno.h>

static void	assign_head(t_tree **root, t_tree *head)
{
	t_tree	*tmp;

	if (!root || !*root || !head)
		return ;
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
	new_node->priority = PRIO_IGNORE;
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

void	add_to_root(t_token **node, t_tree **root)
{
	t_token	*end;
	t_token	*start;

	if (!*node)
		return ;
	handle_parenthesis(node);
	end = (*node);
	start = (*node);
	assign_priority(node);
	last_node(&end);
	if (start->prev)
		start->priority = 0;
	*root = parse_list(*node, end);
	handle_boolop_group(root);
	if (!*root)
	{
		errno = MEM_ALLOC;
		return ;
	}
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
		if (tmp_token->group)
			free_parse(tmp_token->group, NULL, 0);
		if (tmp_token->content)
			free_tab(tmp_token->content);
		free(tmp_token);
		tmp_token = NULL;
	}
	if (tmp_head)
		free(tmp_head);
}
