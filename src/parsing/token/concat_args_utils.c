/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:44:43 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 21:26:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "token.h"
#include "quote.h"
#include "tree.h"
#include <errno.h>
#include <stdlib.h>

void	change_redir(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp && errno != MEM_ALLOC)
	{
		if ((tmp->token == R_IN || tmp->token == HD
			|| tmp->token == APPEND || tmp->token == TRUNC)
			&& is_same_family_redir(tmp))
			handle_change_node(&tmp, true);
		else
			tmp = tmp->next;
	}
}

bool	is_same_family_redir(t_token *node)
{
	if ((node->token == APPEND)
		|| (node->token == HD)
		|| (node->token == R_IN) || (node->token == TRUNC))
	{
		if (!node->content[1] && node->next
			&& node->next->token != APPEND && node->next->token != HD
			&& node->next->token != R_IN && node->next->token != TRUNC)
			return (true);
	}
	return (false);
}

void	change_node(t_token **node, t_token *next_node,
						char **new_content, bool flag)
{
	free_tab((*node)->content);
	(*node)->content = new_content;
	if (flag)
	{
		(*node)->next->next = next_node->next;
		if ((*node)->next->next)
			(*node)->next->next->prev = (*node)->next;
		free_token(&next_node);
	}
	else
	{
		(*node)->next = next_node->next;
		if ((*node)->next)
			(*node)->next->prev = *node;
		free_tab(next_node->content);
		free(next_node);
		if ((*node)->token == APPEND
			|| (*node)->token == HD
			|| (*node)->token == R_IN || (*node)->token == TRUNC)
			*node = (*node)->next;
	}
}

bool	is_valid_prev(t_token *prev)
{
	if (!prev)
		return (true);
	if (prev->token == NO_TOKEN)
	{
		if (prev->prev && prev->prev->token == SPACE)
		{
			if (prev->prev->prev && prev->prev->prev->token == HD)
				return (true);
		}
	}
	if (prev->token == SPACE)
		return (is_valid_prev(prev->prev));
	if ((prev->token == PIPE
			|| prev->token == O_OR
			|| prev->token == O_AND
			|| prev->token == R_PARENTHESIS
			|| prev->token == L_PARENTHESIS
			|| prev->token == DIREC
			|| prev->token == FLE))
		return (true);
	return (false);
}

void	handle_is_command(t_token *node, char *cmd_w_path, bool flag)
{
	if (cmd_w_path && (node->error == SUCCESS || node->error == QUOTE))
	{
		if (node->prev && (node->prev->token == R_IN
				|| node->prev->token == HD
				|| node->prev->token == APPEND
				|| node->prev->token == TRUNC))
		{
			free(cmd_w_path);
			return ;
		}
		if (!is_valid_prev(node->prev))
		{
			node->error = CMD_NOT_FOUND;
			return ;
		}
		if (flag)
			replace_tab(&node, cmd_w_path);
		node->token = CMD;
	}
}
