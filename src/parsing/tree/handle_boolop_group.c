/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_boolop_group.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:23:46 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 03:06:54 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "tree.h"
#include "libft.h"

static int	set_boolop_pointer(t_tree **ast_node,
								t_token *token_node, t_token *new_token)
{
	if (token_node->next)
	{
		token_node->next->prev = new_token;
		new_token->next = token_node->next;
		token_node->next = new_token;
	}
	else
		new_token->next = NULL;
	free_tab((*ast_node)->token->content);
	free((*ast_node)->token);
	(*ast_node)->token = new_token;
	(*ast_node)->token->group = NULL;
	new_token->prev = token_node;
	(*ast_node)->token->token = GROUP_BOOLOP;
	free_partial_tree((*ast_node)->left);
	free_partial_tree((*ast_node)->right);
	(*ast_node)->left = NULL;
	(*ast_node)->right = NULL;
	return (0);
}

static int	set_boolop_group(t_tree **ast_node, t_token *token_node)
{
	int		len;
	int		pos;
	char	*res;
	t_token	*new_token;

	len = 0;
	pos = 0;
	len_boolop_group(*ast_node, &len);
	res = ft_calloc(sizeof(char), len + 1);
	if (!res)
		return (-1);
	copy_boolop_group_content(*ast_node, &res, &pos);
	if (pos > 0 && res[pos - 1] == ' ')
		res[pos - 1] = '\0';
	new_token = add_new_token(res, SUCCESS);
	if (!new_token)
		return (-1);
	set_boolop_pointer(ast_node, token_node, new_token);
	free(res);
	return (0);
}

static int	handle_boolop_group_right(t_tree **root)
{
	t_tree	*tmp;

	if (!root || !*root)
		return (-1);
	tmp = *root;
	if (tmp->right)
	{
		if ((tmp->token->token == O_AND
				|| tmp->token->token == O_OR)
			&& tmp->right->token->token != O_AND
			&& tmp->right->token->token != O_OR
			&& tmp->right->token->token != GROUP_PARENTHESIS)
		{
			if (set_boolop_group(&tmp->right, tmp->token) == -1)
				return (-1);
		}
		else
			handle_boolop_group_right(&tmp->right);
	}
	return (0);
}

int	handle_boolop_group(t_tree **root)
{
	t_tree	*tmp;

	if (!root || !*root)
		return (-1);
	tmp = *root;
	if (tmp->left)
	{
		if ((tmp->token->token == O_AND
				|| tmp->token->token == O_OR)
			&& tmp->left->token->token != O_AND
			&& tmp->left->token->token != O_OR
			&& tmp->left->token->token != GROUP_PARENTHESIS)
		{
			if (set_boolop_group(&tmp->left, tmp->token) == -1)
				return (-1);
		}
		else
			handle_boolop_group(&tmp->left);
	}
	handle_boolop_group_right(&tmp);
	return (0);
}
