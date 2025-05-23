/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:26:02 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 18:38:29 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"

static bool	token_valid_for_join(t_type token)
{
	if (token == O_AND || token == O_OR
		|| token == R_IN || token == HD || token == APPEND
		|| token == TRUNC || token == PIPE
		|| token == L_PARENTHESIS || token == R_PARENTHESIS)
		return (false);
	else
		return (true);
}

void	join_token(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (token_valid_for_join(tmp->token) && tmp->next && tmp->next->token != SPACE)
			change_node(&tmp);
		else
			tmp = tmp->next;
	}
}
