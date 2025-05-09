/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:58:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/09 22:41:58 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "expand.h"
#include "list.h"
#include "libft.h"

void	delete_space_node(t_token **head)
{
	t_token	*tmp;
	t_token	*old_node;

	if (!*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if ((tmp->next || tmp->prev)
			&& (tmp->token == SPACE || !tmp->content[0][0]))
		{
			old_node = tmp;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (*head == tmp)
				*head = tmp->next;
			tmp = tmp->next;
			free_tab(old_node->content);
			free(old_node);
		}
		else
			tmp = tmp->next;
	}
}

static void	add_space(t_token **node)
{
	char	*res;

	res = ft_strjoin((*node)->content[0], " ");
	if (!res)
		free_parse(*node, "Malloc faile in function add_space", MEM_ALLOC);
	free((*node)->content[0]);
	(*node)->content[0] = res;
}

static void	handle_space_for_echo(t_token *node)
{
	while (node && (node->token == S_QUOTE || node->token == D_QUOTE
			|| node->token == SPACE || node->token == EXPAND
			|| node->token == NO_TOKEN || node->token == DIREC
			|| node->token == FLE || node->error != 0))
	{
		if ((node->token == S_QUOTE || node->token == EXPAND
				|| node->error != 0)
			&& (node->next && node->next->token == SPACE
				&& ft_strncmp(node->content[0], "-n", 3)))
		{
			if (node->next->next && (node->next->next->token != R_IN
					&& node->next->next->token != HD
					&& node->next->next->token != TRUNC
					&& node->next->next->token != APPEND
					&& node->next->next->token != O_AND
					&& node->next->next->token != O_OR
					&& node->next->next->token != PIPE))
				add_space(&node);
		}
		node = node->next;
	}
}

void	handle_space(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->token == BUILT_IN && !ft_strncmp(tmp->content[0], "echo", 5))
		{
			tmp = tmp->next;
			handle_space_for_echo(tmp);
		}
		else
			tmp = tmp->next;
	}
}
