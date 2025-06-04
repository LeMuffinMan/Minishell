/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:19:19 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:13:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "quote.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>

bool	syntax_error_for_op(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (case_is_boolop(tmp, head))
			return (true);
		if (case_is_pipe(tmp, head))
			return (true);
		if (case_is_left_parenthesis(tmp, head))
			return (true);
		if (case_is_right_parenthesis(tmp, head))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

static t_token	*set_quote_or_par_error(t_token *node, t_type token)
{
	t_token	*head;
	char	*operator;
	char	*tmp;
	char	*new_content;

	if (!node || !node->content)
		return (NULL);
	alloc_operator_for_error(token, &operator);
	if (!operator)
		return (NULL);
	free_parse(node);
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

bool	error_one_quote(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return (true);
	tmp = *head;
	while (tmp)
	{
		if (tmp->error == PB_QUOTE)
		{
			*head = set_quote_or_par_error(tmp, tmp->token);
			if (!*head)
			{
				errno = MEM_ALLOC;
				return (false);
			}
			return (true);
		}
		tmp = tmp->next;
	}
	return (true);
}

bool	error_one_parenthesis(t_token **head)
{
	t_token	*tmp;
	int		left;
	int		right;

	if (!head || !*head)
		return (true);
	tmp = *head;
	left = 0;
	right = 0;
	while (tmp)
	{
		if (tmp->token == L_PARENTHESIS)
			left++;
		else if (tmp->token == R_PARENTHESIS)
			right++;
		tmp = tmp->next;
	}
	if (left > right)
		*head = set_quote_or_par_error(*head, R_PARENTHESIS);
	else if (left < right)
		*head = set_quote_or_par_error(*head, L_PARENTHESIS);
	if (!*head)
		return (false);
	return (true);
}

void	check_syntax_error(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	if (syntax_error_for_op(head))
		return ;
	while (tmp)
	{
		if ((((tmp->token == R_IN) || (tmp->token == HD)
					|| (tmp->token == TRUNC) || (tmp->token == APPEND))
				&& !tmp->content[1]))
		{
			*head = set_syntax_error(tmp);
			if (!*head)
				errno = MEM_ALLOC;
			return ;
		}
		else if (tmp->token == DIREC)
		{
			case_of_directory_error(&tmp);
			return ;
		}
		tmp = tmp->next;
	}
}
