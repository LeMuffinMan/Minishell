/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 04:50:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 00:33:21 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include "expand.h"
#include "structs.h"
#include <stdlib.h>
#include <errno.h>

static bool	is_same_family(t_token *node)
{
	if ((node->token == APPEND)
		|| (node->token == HD)
		|| (node->token == R_IN) || (node->token == TRUNC))
	{
		if (node->next && node->next->token != APPEND
			&& node->next->token != HD
			&& node->next->token != R_IN && node->next->token != TRUNC)
			return (true);
	}
	else if ((node->token == BUILT_IN) || (node->token == CMD))
	{
		if (node->next)
		{
			if ((node->next->error != 0) || (node->next->token == CMD)
				|| (node->next->token == D_QUOTE)
				|| (node->next->token == S_QUOTE)
				|| (node->next->token == EXPAND)
				|| (node->next->token == DIREC)
				|| (node->next->token == FLE)
				|| (node->next->token == WILDCARD))
				return (true);
		}
	}
	return (false);
}

static char	**join_node_content(t_token *node, char **old, char **new)
{
	char	**res;
	int		len_old;
	int		len_new;

	len_old = ft_tab_len(old);
	len_new = ft_tab_len(new);
	res = malloc(sizeof(char *) * (len_old + len_new + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		free_parse(node, NULL, MEM_ALLOC);
		return (NULL);
	}
	res = copy_tab(node, res, old, 0);
	if (!res)
		return (NULL);
	res = copy_tab(node, res, new, len_old);
	if (!res)
		return (NULL);
	res[len_new + len_old] = NULL;
	return (res);
}

void	change_node(t_token **node, bool flag)
{
	t_token	*next_node;
	char	**new_content;

	next_node = (*node)->next;
	if (!next_node)
		return ;
	if (flag)
		new_content = join_node_content(*node,
				(*node)->content, next_node->content);
	else
		new_content = join_content(*node,
				(*node)->content, next_node->content);
	if (!new_content)
		return ;
	free_tab((*node)->content);
	(*node)->content = new_content;
	(*node)->next = next_node->next;
	if ((*node)->next)
		(*node)->next->prev = *node;
	free_tab(next_node->content);
	free(next_node);
	if (((*node)->token == APPEND)
		|| ((*node)->token == HD)
		|| ((*node)->token == R_IN) || ((*node)->token == TRUNC))
		*node = (*node)->next;
}

bool	handle_expand_and_join(t_token **head, t_var *list_env,
								t_lists *lists, bool flag)
{
	if (flag)
	{
		if (init_expand(head, list_env, lists))
		{
			if (errno == MEM_ALLOC)
			{
				free_parse(*head, NULL, MEM_ALLOC);
				return (false);
			}
			if (!parse_again(head, list_env, &flag))
			{
				free_parse(*head, NULL, MEM_ALLOC);
				errno = MEM_ALLOC;
				return (false);
			}
		}
	}
	if (join_token(head))
		assign_token(head, list_env, true);
	delete_space_node(head);
	return (true);
}

bool	concat_args(t_token **head, t_var *list_env, bool flag, t_lists *lists)
{
	t_token	*tmp;

	if (!head || !*head)
		return (true);
	if (!handle_expand_and_join(head, list_env, lists, flag))
	{
		errno = MEM_ALLOC;
		return (false);
	}
	tmp = *head;
	errno = SUCCESS;
	while (tmp && errno != MEM_ALLOC)
	{
		if (is_same_family(tmp))
			change_node(&tmp, true);
		else
			tmp = tmp->next;
	}
	check_syntax_error(head);
	if (!head || !*head)
	{
		errno = MEM_ALLOC;
		return (false);
	}
	return (true);
}
