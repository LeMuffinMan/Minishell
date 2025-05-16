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

static int	find_len_new_content(t_token *node, t_token *end)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (node)
	{
		i = -1;
		while (node->content[++i])
			len += ft_strlen(node->content[i]) + 1;
		if (node == end)
			break ;
		node = node->next;
	}
	return (len);
}

static void	copy_nodes_content(t_token *start, t_token *end, char **res,
				t_type current_token)
{
	int		pos;
	int		i;
	int 	word_len;

	pos = 0;
	while (start)
	{
		i = -1;
		while (start->content[++i])
		{
			if (current_token == start->token
				|| start->token == R_PARENTHESIS)
				break ;
			word_len = ft_strlen(start->content[i]);
			ft_memcpy(*res + pos, start->content[i], word_len);
			pos += word_len;
			(*res)[pos++] = ' ';
		}
		if (start == end)
			break ;
		start = start->next;
	}
	if (pos > 0 && (*res)[pos - 1] == ' ')
		(*res)[pos - 1] = '\0';
}

static void	free_node_unused(t_token *node)
{
	t_token	*next_node;

	if (node->next)
		next_node = node->next;
	while (node)
	{
		next_node = node->next;
		if (node->content)
			free_tab(node->content);
		if (node)
			free(node);
		node = NULL;
		node = next_node;
	}
}

// need to resolve leak when we have some groups
static void	assign_pointer(t_token **new_node, t_token **start, t_token **end)
{
	char	*res;
	int		i;

	i = find_len_new_content(*start, *end);
	res = ft_calloc(sizeof(char), i + 1);
	if (!res)
		free_parse(*new_node, "Malloc failed in function 'assign_pointer'", MEM_ALLOC);
	copy_nodes_content(*start, *end, &res, (*start)->token);
	(*new_node)->prev = (*start)->prev;
	(*new_node)->next = (*end)->next;
	if ((*new_node)->prev)
		(*new_node)->prev->next = (*new_node);
	if ((*new_node)->next)
		(*new_node)->next->prev = (*new_node);
	(*new_node)->group = add_new_token(res, SUCCESS);
	free(res);
	if ((*start)->token == L_PARENTHESIS)
		(*new_node)->token = GROUP_PARENTHESIS;
	else if ((*start)->token == O_AND)
		(*new_node)->token = GROUP_O_AND;
	else
		(*new_node)->token = GROUP_O_OR;
	(*start)->prev = NULL;
	(*end)->next = NULL;
	free_node_unused(*start);
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
			"Malloc failed in 'add_new_token' for group", MEM_ALLOC);
	assign_pointer(&tmp, start, end);
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
