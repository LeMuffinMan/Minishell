/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:31:22 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 23:46:22 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "quote.h"
#include "libft.h"
#include <errno.h>

void	case_of_directory_error(t_token **node)
{
	int	len;

	len = ft_strlen((*node)->content[0]);
	if ((*node)->content[0][len - 1] == '/')
		(*node)->error = IS_A_DIR;
	else
		(*node)->error = CMD_NOT_FOUND;
}

void	alloc_operator_for_error(t_type token, char **operator)
{
	if (token == S_QUOTE)
		*operator = ft_strdup("'");
	else if (token == D_QUOTE)
		*operator = ft_strdup("\"");
	else if (token == L_PARENTHESIS)
		*operator = ft_strdup("(");
	else
		*operator = ft_strdup(")");
}

t_token	*set_syntax_error(t_token *node)
{
	t_token	*head;
	char	*operator;
	char	*tmp;
	char	*new_content;

	if (!node || !node->content || !node->content[0])
		return (NULL);
	operator = ft_strdup(node->content[0]);
	if (!operator)
		return (NULL);
	free_parse(node, NULL, 0);
	tmp = ft_strjoin("syntax error near unexpected token `", operator);
	free(operator);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "'");
	free(tmp);
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, NO_F_OR_D, false);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}

t_token	*set_parenthesis_error(t_token *node)
{
	t_token	*head;
	char	*new_content;

	if (!node || !node->content || !node->content[0])
		return (NULL);
	free_parse(node, NULL, 0);
	new_content = ft_strdup(
			"Usage: Use parenthesis for priorities of '&&' and '||'\n");
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, E_IO, false);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}
