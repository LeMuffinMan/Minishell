/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:42:07 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:18:22 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "token.h"
#include "libft.h"
#include "expand.h"
#include <errno.h>

static bool	is_valid_quote(t_token *node)
{
	if ((node->token == D_QUOTE && node->content
			&& node->content[0] && node->content[0][0]))
		return (true);
	return (false);
}

static bool	to_expand(t_token **node, t_var *list_env, t_lists *lists)
{
	int		i;
	bool	flag;

	i = -1;
	flag = false;
	while ((*node)->content[0][++i] && errno != MEM_ALLOC)
	{
		if ((*node)->content[0][i] == '$' && ((*node)->content[0][i + 1]
			|| ((*node)->next && ((*node)->next->token == S_QUOTE
			|| is_valid_quote((*node)->next)))))
		{
			if ((i > 0 && (*node)->content[0][i - 1] == '\\')
			|| ((*node)->content[0][i + 1]
			&& ft_isspace((*node)->content[0][i + 1])))
				i++;
			else
			{
				expand_node_content(node, list_env, i + 1, lists);
				if ((*node)->content[0][0])
					flag = true;
				i = -1;
			}
		}
	}
	return (flag);
}

bool	init_expand(t_token **head, t_var *list_env, t_lists *lists)
{
	t_token	*tmp;
	bool	flag;

	tmp = *head;
	flag = false;
	while (tmp)
	{
		if (tmp->token == S_QUOTE || (tmp->prev
				&& tmp->prev->token == HD)
			|| (tmp->prev && tmp->prev->prev
				&& tmp->prev->prev->token == HD))
			tmp = tmp->next;
		else
		{
			flag = to_expand(&tmp, list_env, lists);
			if (errno == MEM_ALLOC)
				return (false);
			tmp = tmp->next;
		}
	}
	handle_space(head);
	return (flag);
}
