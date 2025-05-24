/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:53:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 03:09:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "token.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>

static bool	assign_pointer(t_token **new_node, t_token **start, t_token **end)
{
	char	*res;
	int		i;

	i = find_len_new_content(*start, *end);
	res = ft_calloc(sizeof(char), i + 1);
	if (!res)
		return (false);
	copy_nodes_content(*start, *end, &res, (*start)->token);
	(*new_node)->prev = (*start)->prev;
	(*new_node)->next = (*end)->next;
	if ((*new_node)->prev)
		(*new_node)->prev->next = (*new_node);
	if ((*new_node)->next)
		(*new_node)->next->prev = (*new_node);
	(*new_node)->group = add_new_token(res, SUCCESS);
	if (!res)
		return (false);
	free(res);
	(*new_node)->token = GROUP_PARENTHESIS;
	(*start)->prev = NULL;
	(*end)->next = NULL;
	free_node_unused(*start);
	return (true);
}

static t_token	*set_group_parenthesis(t_token **start, t_token **end,
					t_token **current)
{
	t_token	*new_head;
	t_token	*tmp;
	bool	flag;

	flag = false;
	new_head = *start;
	if (new_head->prev)
	{
		while (new_head->prev)
			new_head = new_head->prev;
	}
	else
		flag = true;
	tmp = add_new_token("()", SUCCESS);
	if (!tmp || !assign_pointer(&tmp, start, end))
	{
		free_parse(*start, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return (NULL);
	}
	(*current) = tmp;
	if (flag)
		new_head = tmp;
	return (new_head);
}

void	handle_parenthesis(t_token **head)
{
	t_token	*tmp;
	t_token	*left_par;
	t_token	*right_par;

	tmp = *head;
	left_par = NULL;
	right_par = NULL;
	errno = SUCCESS;
	while (tmp && errno != MEM_ALLOC)
	{
		if (tmp->token == L_PARENTHESIS)
		{
			left_par = tmp;
			while (tmp && tmp->token != R_PARENTHESIS)
				tmp = tmp->next;
			right_par = tmp;
			*head = set_group_parenthesis(&left_par, &right_par, &tmp);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
