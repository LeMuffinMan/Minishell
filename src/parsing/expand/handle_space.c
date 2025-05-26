/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:58:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 03:12:00 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "expand.h"
#include "list.h"
#include "libft.h"
#include <errno.h>

void	delete_space_node(t_token **head)
{
	t_token	*tmp;
	t_token	*old_node;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == SPACE || !tmp->content || !tmp->content[0]
			|| !tmp->content[0][0])
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

static void	handle_space_for_echo(t_token **node)
{
	t_token	*tmp;

	tmp = *node;
	while (tmp && errno != MEM_ALLOC && (tmp->token == S_QUOTE
			|| tmp->token == D_QUOTE
			|| tmp->token == SPACE || tmp->token == EXPAND
			|| tmp->token == NO_TOKEN || tmp->token == DIREC
			|| tmp->token == FLE || tmp->token == ARG || tmp->error != 0))
	{
		if ((tmp->token == S_QUOTE || tmp->token == EXPAND
				|| tmp->error != 0)
			&& (tmp->next && tmp->next->token == SPACE
				&& ft_strncmp(tmp->content[0], "-n", 2)))
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

static void	add_space_for_export(t_token **node)
{
	int		new_content;
	char	*res;
	int		len_current;
	int		len_next;

	len_current = ft_strlen((*node)->content[0]);
	len_next = ft_strlen((*node)->next->content[0]);
	new_content = len_current + len_next;
	res = malloc(sizeof(char) * (new_content + 1));
	if (!res)
	{
		free_parse((*node), NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return ;
	}
	ft_memcpy(res, (*node)->content[0], len_current);
	ft_memcpy(res + len_current, (*node)->next->content[0], len_next);
	res[new_content] = '\0';
	free((*node)->content[0]);
	(*node)->content[0] = res;
}

static void	handle_space_for_export(t_token **node)
{
	t_token	*tmp;

	if (!node || !*node)
		return ;
	tmp = *node;
	while (tmp && errno != MEM_ALLOC
		&& (tmp->error != 0 || tmp->token == SPACE))
	{
		if (tmp->error != 0
			&& ft_strlen(tmp->content[0]) > 0
			&& tmp->content[0][ft_strlen(tmp->content[0]) - 1] == '='
			&& tmp->next && tmp->next->content && tmp->next->content[0]
			&& verif_is_token_valid(tmp->next->token))
		{
			add_space_for_export(&tmp);
			if (errno == MEM_ALLOC)
				return ;
			delete_node_pointer(&tmp);
		}
		else
			tmp = tmp->next;
	}
}

void	handle_space(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp && errno != MEM_ALLOC)
	{
		if (!ft_strncmp(tmp->content[0], "echo", 5))
		{
			tmp = tmp->next;
			if (tmp && tmp->token == SPACE)
				tmp = tmp->next;
			if (tmp && tmp->token == EXPAND && tmp->error != 200)
				delete_space_content(&tmp);
			handle_space_for_echo(&tmp);
		}
		else if (!ft_strncmp(tmp->content[0], "export", 7))
		{
			tmp = tmp->next;
			if (tmp && tmp->token == SPACE)
				tmp = tmp->next;
			handle_space_for_export(&tmp);
		}
		else
			tmp = tmp->next;
	}
}
