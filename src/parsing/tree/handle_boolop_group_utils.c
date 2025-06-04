/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_boolop_group_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:04:14 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 22:36:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "tree.h"
#include "libft.h"

static bool	char_is_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

void	len_boolop_group(t_tree *ast_node, int *len)
{
	int	i;

	if (!ast_node || !ast_node->token || !ast_node->token->content)
		return ;
	i = 0;
	if (ast_node->left)
		len_boolop_group(ast_node->left, len);
	if (ast_node->right)
		len_boolop_group(ast_node->right, len);
	while (ast_node->token->content[i])
	{
		*len += ft_strlen(ast_node->token->content[i]) + 1;
		if (char_is_equal(ast_node->token->content[i]))
			*len += 2;
		i++;
	}
}

static void	case_is_ex_value(t_tree *ast_node, char **res, int *pos, int i)
{
	int		word_len;
	int		j;

	j = 0;
	word_len = ft_strlen(ast_node->token->content[i]);
	while (j < word_len)
	{
		(*res)[*pos] = ast_node->token->content[i][j];
		if (ast_node->token->content[i][j] == '=')
		{
			(*pos)++;
			(*res)[*pos] = '"';
		}
		j++;
		(*pos)++;
	}
	(*res)[(*pos)++] = '"';
	(*res)[(*pos)++] = ' ';
}

void	copy_boolop_group_content(t_tree *ast_node, char **res, int *pos)
{
	int	i;
	int	word_len;

	if (!ast_node || !ast_node->token || !ast_node->token->content)
		return ;
	i = -1;
	if (ast_node->left)
		copy_boolop_group_content(ast_node->left, res, pos);
	while (ast_node->token->content[++i])
	{
		if (char_is_equal(ast_node->token->content[i]))
			case_is_ex_value(ast_node, res, pos, i);
		else
		{
			word_len = ft_strlen(ast_node->token->content[i]);
			ft_memcpy(*res + *pos, ast_node->token->content[i], word_len);
			*pos += word_len;
			(*res)[(*pos)++] = ' ';
		}
	}
	if (ast_node->right)
		copy_boolop_group_content(ast_node->right, res, pos);
}

void	free_partial_tree(t_tree *node)
{
	if (!node)
		return ;
	free_partial_tree(node->left);
	free_partial_tree(node->right);
	if (node->token)
	{
		free_tab(node->token->content);
		free(node->token);
	}
	free(node);
}
