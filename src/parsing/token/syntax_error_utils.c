/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:31:22 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:02:10 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "quote.h"
#include "libft.h"
#include <errno.h>

// char	*sort_of_token(t_type token)
// {
// 	char	*operator;

// 	if (token == S_QUOTE)
// 		operator = ft_strdup("'");
// 	else if (token == D_QUOTE)
// 		operator = ft_strdup("\"");
// 	else if (token == L_PARENTHESIS)
// 		operator = ft_strdup("(");
// 	else
// 		operator = ft_strdup(")");
// 	return (operator);
// }
// bool	syntax_error_for_boolop(t_token *node)
// {
// 	if (!node->prev || !node->next
// 		|| (node->prev->token != CMD && node->prev->token != BUILT_IN
// 			&& node->prev->token != R_PARENTHESIS && node->prev->token != NO_TOKEN
// 			&& node->prev->token != D_QUOTE && node->prev->token != S_QUOTE)
// 		|| (node->next->token != CMD && node->next->token != BUILT_IN
// 			&& node->next->token != L_PARENTHESIS && node->next->token != NO_TOKEN 
// 			&& node->next->token != D_QUOTE && node->next->token != S_QUOTE))
// 		return (true);
// 	return (false);
// }

// bool	syntax_error_for_parenthesis(t_token *prev_o_next)
// {
// 	if (prev_o_next
// 		&& prev_o_next->token != O_AND && prev_o_next->token != O_OR
// 		&& prev_o_next->token != PIPE && prev_o_next->token != L_PARENTHESIS)
// 		return (true);
// 	return (false);
// }


// bool	error_one_quote(t_token **head)
// {
// 	t_token	*tmp;

// 	if (!head || !*head)
// 		return (true);
// 	tmp = *head;
// 	while (tmp)
// 	{
// 		if (tmp->error == PB_QUOTE)
// 		{
// 			*head = set_quote_or_par_error(tmp, tmp->token);
// 			if (*head)
// 			{
// 				errno = MEM_ALLOC;
// 				return (false);
// 			}
// 			return (true);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (true);
// }

// bool	error_one_parenthesis(t_token **head)
// {
// 	t_token	*tmp;
// 	int		left;
// 	int		right;

// 	if (!head || !*head)
// 		return (true);
// 	tmp = *head;
// 	left = 0;
// 	right = 0;
// 	while (tmp)
// 	{
// 		if (tmp->token == L_PARENTHESIS)
// 			left++;
// 		else if (tmp->token == R_PARENTHESIS)
// 			right++;
// 		tmp = tmp->next;
// 	}
// 	if (left > right)
// 		*head = set_quote_or_par_error(*head, R_PARENTHESIS);
// 	else if (left < right)
// 		*head = set_quote_or_par_error(*head, L_PARENTHESIS);
// 	if (!*head)
// 		return (false);
// 	return (true);
// }
