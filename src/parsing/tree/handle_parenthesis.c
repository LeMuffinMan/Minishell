/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:53:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/16 17:41:16 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "token.h"
#include "libft.h"
#include <stdlib.h>

// static void	delete_parenthesis_node(t_token **group)
// {
// 	t_token	*tmp;
// 	t_token	*next_node;

// 	tmp = *group;
// 	next_node = tmp->next;
// 	next_node->prev = NULL;
// 	free_tab(tmp->content);
// 	free(tmp);
// 	tmp = next_node;
// 	*group = tmp;
// 	while (tmp->next)
// 		tmp = tmp->next;
// 	next_node = tmp->prev;
// 	next_node->next = NULL;
// 	free_tab(tmp->content);
// 	free(tmp);
// 	tmp = next_node;
// }

static void	assign_pointer(t_token **new_node, t_token **start, t_token **end)
{
	char	*res;
	int		i;
	int		len;
	t_token	*tmp_node;
	int		pos;

	tmp_node = *start;
	len = 0;
	while (tmp_node)
	{
		i = -1;
		while (tmp_node->content[++i])
			len += ft_strlen(tmp_node->content[i]) + 1;
		if (tmp_node == *end)
			break ;
		tmp_node = tmp_node->next;
	}
	res = ft_calloc(sizeof(char), len + 1);
	if (!res)
		free_parse(*new_node, "Malloc failed in function 'assign_pointer'", MEM_ALLOC);
	tmp_node = *start;
	pos = 0;
	while (tmp_node)
	{
		i = -1;
		while (tmp_node->content[++i])
		{
			int word_len = ft_strlen(tmp_node->content[i]);
			ft_memcpy(res + pos, tmp_node->content[i], word_len);
			pos += word_len;
			res[pos++] = ' ';
		}
		if (tmp_node == *end)
			break ;
		tmp_node = tmp_node->next;
	}
	if (pos > 0 && res[pos - 1] == ' ')
		res[pos - 1] = '\0';
	(*new_node)->prev = (*start)->prev;
	(*new_node)->next = (*end)->next;
	if ((*new_node)->prev)
		(*new_node)->prev->next = (*new_node);
	if ((*new_node)->next)
		(*new_node)->next->prev = (*new_node);
	(*new_node)->group = add_new_token(res, SUCCESS);
	(*new_node)->token = GROUP_PARENTHESIS;
	(*start)->prev = NULL;
	(*end)->next = NULL;
	// delete_parenthesis_node(&(*new_node)->group);
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
	if (!tmp)
		free_parse(*start,
			"Malloc failed in 'add_new_token' for group parenthesis",
			MEM_ALLOC);
	assign_pointer(&tmp, start, end);
	(*current) = tmp;
	if (flag)
		new_head = tmp;
	return (new_head);
}

void	handle_bool_operator(t_token **head)
{
	t_token	*tmp;
	// t_token	*start;
	// t_token	*end;
	// t_type	current;

	tmp = *head;
	return ;
	// start = NULL;
	// end = NULL;
	// while (tmp)
	// {
	// 	if (tmp->token == O_AND || tmp->token == O_OR)
	// 	{
	// 		current = tmp->token;
	// 		start = tmp;
	// 		while (tmp && tmp->token != current)
	// 			tmp = tmp->prev;
	// 		end = tmp;
	// 		*head = set_group_parenthesis(&start, &end, &tmp);
	// 		tmp = tmp->next;
	// 	}
	// 	else
	// 		tmp = tmp->next;
	// }
}

void	handle_parenthesis(t_token **head)
{
	t_token	*tmp;
	t_token	*left_par;
	t_token	*right_par;

	tmp = *head;
	left_par = NULL;
	right_par = NULL;
	while (tmp)
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
