/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:19:19 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:02:03 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "quote.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>

// t_token	*set_parenthesis_error(t_token *node)
// {
// 	t_token	*head;
// 	char	*new_content;

// 	if (!node || !node->content || !node->content[0])
// 		return (NULL);
// 	free_parse(node, NULL, 0);
// 	new_content = ft_strdup("Usage: Use parenthesis for priorities of '&&' and '||'\n");
// 	if (!new_content)
// 		return (NULL);
// 	head = add_new_token(new_content, E_IO);
// 	free(new_content);
// 	if (!head)
// 		return (NULL);
// 	return (head);
// }

// bool	syntax_error_for_op(t_token **head)
// {
// 	t_token	*tmp;

// 	tmp = *head;
// 	while (tmp)
// 	{
// 		if (tmp->token == O_OR || tmp->token == O_AND || tmp->token == PIPE)
// 		{
// 			if (!tmp->prev || !tmp->next
// 				|| (tmp->prev->token != CMD && tmp->prev->token != BUILT_IN
// 					&& tmp->prev->token != R_PARENTHESIS && tmp->prev->token != NO_TOKEN
// 					&& tmp->prev->token != D_QUOTE && tmp->prev->token != S_QUOTE)
// 				|| (tmp->next->token != CMD && tmp->next->token != BUILT_IN
// 					&& tmp->next->token != L_PARENTHESIS && tmp->next->token != NO_TOKEN 
// 					&& tmp->next->token != D_QUOTE && tmp->next->token != S_QUOTE))
// 			{
// 				*head = set_parenthesis_error(tmp);
// 				return (true);
// 			}
// 		}
// 		if (tmp->token == L_PARENTHESIS)
// 		{
// 			if (tmp->prev
// 				&& tmp->prev->token != O_AND && tmp->prev->token != O_OR
// 				&& tmp->prev->token != PIPE && tmp->prev->token != L_PARENTHESIS)
// 			{
// 				*head = set_parenthesis_error(tmp);
// 				return (true);
// 			}
// 			if (!tmp->next || tmp->next->token == R_PARENTHESIS)
// 			{
// 				*head = set_parenthesis_error(tmp);
// 				return (true);
// 			}
// 		}
// 		if (tmp->token == R_PARENTHESIS)
// 		{
// 			if (tmp->next
// 				&& tmp->next->token != O_AND && tmp->next->token != O_OR
// 				&& tmp->next->token != PIPE && tmp->next->token != R_PARENTHESIS)
// 			{
// 				*head = set_parenthesis_error(tmp);
// 				return (true);
// 			}
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (false);
// }

// t_token	*set_quote_or_par_error(t_token *node, t_type token)
// {
// 	t_token	*head;
// 	char	*operator;
// 	char	*tmp;
// 	char	*new_content;

// 	if (!node || !node->content)
// 		return (NULL);
// 	operator = sort_of_token(token);
// 	if (!operator)
// 		return (NULL);
// 	free_parse(node, NULL, 0);
// 	tmp = ft_strjoin("syntax error near unexpected token `", operator);
// 	free(operator);
// 	if (!tmp)
// 		return (NULL);
// 	new_content = ft_strjoin(tmp, "'");
// 	free(tmp);
// 	if (!new_content)
// 		return (NULL);
// 	head = add_new_token(new_content, NO_F_OR_D);
// 	free(new_content);
// 	return (head);
// }

// void	check_syntax_error(t_token **head)
// {
// 	t_token	*tmp;

// 	if (!head || !*head)
// 		return ;
// 	tmp = *head;
// 	if (syntax_error_for_op(head))
// 		return ;
// 	while (tmp)
// 	{
// 		if ((((tmp->token == R_IN) || (tmp->token == HD)
// 					|| (tmp->token == TRUNC) || (tmp->token == APPEND))
// 				&& !tmp->content[1]))
// 		{
// 			*head = set_syntax_error(tmp);
// 			return ;
// 		}
// 		else if (tmp->token == DIREC)
// 		{
// 			case_of_directory_error(&tmp);
// 			return ;
// 		}
// 			tmp = tmp->next;
// 	}
// }

// t_token	*set_syntax_error(t_token *node)
// {
// 	t_token	*head;
// 	char	*operator;
// 	char	*tmp;
// 	char	*new_content;

// 	if (!node || !node->content || !node->content[0])
// 		return (NULL);
// 	operator = ft_strdup(node->content[0]);
// 	if (!operator)
// 		return (NULL);
// 	free_parse(node, NULL, 0);
// 	tmp = ft_strjoin("syntax error near unexpected token `", operator);
// 	free(operator);
// 	if (!tmp)
// 		return (NULL);
// 	new_content = ft_strjoin(tmp, "'");
// 	free(tmp);
// 	if (!new_content)
// 		return (NULL);
// 	head = add_new_token(new_content, NO_F_OR_D);
// 	free(new_content);
// 	if (!head)
// 		return (NULL);
// 	return (head);
// }
