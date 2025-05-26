/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings_syntax_error.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 20:30:04 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 23:43:44 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"

bool	case_is_left_parenthesis(t_token *tmp, t_token **head)
{
	if (tmp->token == L_PARENTHESIS)
	{
		if (tmp->prev
			&& tmp->prev->token != O_AND && tmp->prev->token != O_OR
			&& tmp->prev->token != L_PARENTHESIS)
		{
			*head = set_parenthesis_error(tmp);
			return (true);
		}
		if (!tmp->next || tmp->next->token == R_PARENTHESIS)
		{
			*head = set_parenthesis_error(tmp);
			return (true);
		}
	}
	return (false);
}

bool	case_is_right_parenthesis(t_token *tmp, t_token **head)
{
	if (tmp->token == R_PARENTHESIS)
	{
		if (tmp->next
			&& tmp->next->token != O_AND && tmp->next->token != O_OR
			&& tmp->next->token != R_PARENTHESIS)
		{
			*head = set_parenthesis_error(tmp);
			return (true);
		}
	}
	return (false);
}

bool	case_is_boolop(t_token *tmp, t_token **head)
{
	if (tmp->token == O_OR || tmp->token == O_AND)
	{
		if (!tmp->prev || !tmp->next
			|| (tmp->prev->token != CMD && tmp->prev->token != BUILT_IN
				&& tmp->prev->token != R_PARENTHESIS
				&& tmp->prev->token != NO_TOKEN
				&& tmp->prev->token != D_QUOTE && tmp->prev->token != S_QUOTE)
			|| (tmp->next->token != CMD && tmp->next->token != BUILT_IN
				&& tmp->next->token != L_PARENTHESIS
				&& tmp->next->token != NO_TOKEN
				&& tmp->next->token != D_QUOTE && tmp->next->token != S_QUOTE))
		{
			*head = set_syntax_error(tmp);
			return (true);
		}
	}
	return (false);
}

bool	case_is_pipe(t_token *tmp, t_token **head)
{
	if (tmp->token == PIPE && (!tmp->prev || !tmp->next
			|| (tmp->prev->token != CMD && tmp->prev->token != BUILT_IN
				&& tmp->prev->error == CMD_NOT_FOUND
				&& tmp->prev->error == PERMISSION_DENIED)
			|| (tmp->next->token != CMD
				&& tmp->next->token != BUILT_IN
				&& tmp->next->error == CMD_NOT_FOUND
				&& tmp->next->error == PERMISSION_DENIED)))
	{
		*head = set_syntax_error(tmp);
		return (true);
	}
	return (false);
}
