/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:58:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 15:13:40 by asinsard         ###   ########lyon.fr   */
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
static void	handle_space_for_echo(t_token **node)
{
	t_token	*tmp;
	
	tmp = *node;
	while (tmp && (tmp->token == S_QUOTE || tmp->token == D_QUOTE
			|| tmp->token == SPACE || tmp->token == EXPAND
			|| tmp->token == NO_TOKEN || tmp->token == DIREC
			|| tmp->token == FLE || tmp->error != 0))
	{
		if ((tmp->token == S_QUOTE || tmp->token == EXPAND
				|| tmp->error != 0)
			&& (tmp->next && tmp->next->token == SPACE
				&& ft_strncmp(tmp->content[0], "-n", 3)))
		{
			if (tmp->next->next && (tmp->next->next->token != R_IN
					&& tmp->next->next->token != HD
					&& tmp->next->next->token != TRUNC
					&& tmp->next->next->token != APPEND
					&& tmp->next->next->token != O_AND
					&& tmp->next->next->token != O_OR
					&& tmp->next->next->token != PIPE))
					add_space(&tmp);
		}
		tmp = tmp->next;
	}
}

void	handle_space(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->token == EXPAND && tmp->error != 200)
			delete_space_content(&tmp);
		if (tmp->token == BUILT_IN && !ft_strncmp(tmp->content[0], "echo", 5))
		{
			tmp = tmp->next;
			handle_space_for_echo(&tmp);
			return ;
		}
		else
			tmp = tmp->next;
	}
}
