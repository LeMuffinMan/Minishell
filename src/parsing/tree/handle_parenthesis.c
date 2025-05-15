/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:53:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/15 20:29:40 by asinsard         ###   ########lyon.fr   */
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

static char	*alloc_cmd_line(char **tab_content)
{
	int		i;
	char	*line;
	char	*tmp;
	char	*res;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (tab_content[i])
	{
		line = ft_strjoin(res, tab_content[i]);
		free(res);
		if (!line)
			return (NULL);
		tmp = ft_strjoin(line, " ");
		free(line);
		if (!tmp)
		{
			free(res);
			return (NULL);
		}
		res = ft_strdup(tmp);
		free(tmp);
		if (!res)
			return (NULL);
		i++;
	}
	return (res);
}

static void	assign_pointer(t_token **new_node, t_token **start, t_token **end)
{
	// int	i;
	char	*res;
	char	*tmp;
	// char	*line;
	t_token	*tmp_node;

	res = ft_strdup("");
	if (!res)
		free_parse(*new_node, "Malloc failed in function '?'", MEM_ALLOC);
	tmp_node = *start;
	tmp = NULL;
	while (tmp_node != *end)
	{
		// i = 0;
		// while (tmp_node->content[i])
		// {
		// 	line = ft_strjoin(res, tmp_node->content[i]);
		// 	free(res);
		// 	if (!line)
		// 		free_parse(*new_node, "Malloc failed in function '?'", MEM_ALLOC);
		// 	tmp = ft_strjoin(line, " ");
		// 	if (!tmp)
		// 	{
		// 		free(res);
		// 		free_parse(*new_node, "Malloc failed in function '?'", MEM_ALLOC);
		// 	}
		// 	res = ft_strdup(tmp);
		// 	free(line);
		// 	free(tmp);
		// 	if (!res)
		// 		free_parse(*new_node, "Malloc failed in function '?'", MEM_ALLOC);
		// 	i++;
		// }
		res = alloc_cmd_line(tmp_node->content);
		tmp_node = tmp_node->next;
	}
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
