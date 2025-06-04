/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 04:50:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/04 18:23:39 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include "expand.h"
#include "structs.h"
#include "wildcard.h"
#include <stdlib.h>
#include <errno.h>

bool	is_same_family(t_token *node)
{
	t_token	*next_node;

	next_node = node->next;
	if (next_node && (next_node->token == APPEND
			|| next_node->token == HD
			|| next_node->token == R_IN
			|| next_node->token == TRUNC))
		next_node = next_node->next;
	if ((node->token == BUILT_IN) || (node->token == CMD))
	{
		if (next_node)
		{
			if ((next_node->error != 0) || (next_node->token == CMD)
				|| (next_node->token == D_QUOTE)
				|| (next_node->token == S_QUOTE)
				|| (next_node->token == EXPAND)
				|| (next_node->token == DIREC)
				|| (next_node->token == FLE)
				|| (next_node->token == WILDCARD)
				|| (next_node->token == ARG))
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

void	handle_change_node(t_token **node, bool flag)
{
	t_token	*next_node;
	char	**new_content;
	bool	is_not_redir;

	is_not_redir = false;
	next_node = (*node)->next;
	if (next_node && (next_node->token == APPEND
			|| next_node->token == HD
			|| next_node->token == R_IN
			|| next_node->token == TRUNC))
	{
		next_node = next_node->next;
		is_not_redir = true;
	}
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
	change_node(node, next_node, new_content, is_not_redir);
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
	if (!handle_wildcard(head, flag))
		return (false);
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
	change_redir(&tmp);
	tmp = *head;
	while (tmp && errno != MEM_ALLOC)
	{
		if (is_same_family(tmp))
			handle_change_node(&tmp, true);
		else
			tmp = tmp->next;
	}
	check_syntax_error(head);
	if (errno == MEM_ALLOC)
		return (false);
	return (true);
}
