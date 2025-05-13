/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:53:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/13 20:27:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "token.h"
#include <stdlib.h>

static void	set_group_parenthesis(t_token **start, t_token **end)
{
	t_token	*new_head;
	t_token	*tmp;

	new_head = *start;
	if (new_head->prev)
	{
		while (new_head->prev)
			new_head = new_head->prev;
	}
	tmp = add_new_token("()", SUCCESS);
	if (!tmp)
		free_parse(*start,
			"Malloc failed in 'add_new_token' for group parenthesis", MEM_ALLOC);
	tmp->prev = (*start)->prev;
	tmp->next = (*end)->next;
	tmp->group = (*start);
	(*start)->prev = NULL;
	(*end)->next = NULL;
	// return (new_head);
}

void	handle_parenthesis(t_token **head)
{
	t_token	*tmp;
	t_token	*left_par;
	t_token	*right_par;
	bool	flag;

	tmp = *head;
	left_par = NULL;
	right_par = NULL;
	flag = false;
	while (tmp)
	{
		if (tmp->token == L_PARENTHESIS)
		{
			left_par = tmp;
			flag = true;
		}
		if (tmp->token == R_PARENTHESIS)	
			right_par = tmp;
		tmp = tmp->next;
	}
	if (flag)
		set_group_parenthesis(&left_par, &right_par);
}
