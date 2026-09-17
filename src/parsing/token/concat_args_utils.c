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
#include "quote.h"
#include <errno.h>
#include <stdlib.h>

static t_token	*skip_spaces_back(t_token *node)
{
	while (node && node->token == SPACE)
		node = node->prev;
	return (node);
}

// True when this node is the target of a redirection: the word right
// after `<`, `>`, `>>` or `<<`.
static bool	is_redir_target(t_token *prev)
{
	t_token	*op;

	op = skip_spaces_back(prev->prev);
	if (!op)
		return (false);
	return (op->token == R_IN || op->token == TRUNC
		|| op->token == APPEND || op->token == HD);
}

// True when nothing before this point in the current simple command is
// already the command. This is what tells `< file cat`, where cat is the
// command, from `cat < file x`, where x is an argument of cat.
static bool	no_command_yet(t_token *prev)
{
	while (prev)
	{
		if (prev->token == PIPE || prev->token == O_AND || prev->token == O_OR
			|| prev->token == L_PARENTHESIS || prev->token == R_PARENTHESIS)
			return (true);
		if (prev->token == CMD || prev->token == BUILT_IN)
			return (false);
		prev = prev->prev;
	}
	return (true);
}

bool	is_valid_prev(t_token *prev)
{
	prev = skip_spaces_back(prev);
	if (!prev)
		return (true);
	if ((prev->token == PIPE
			|| prev->token == O_OR
			|| prev->token == O_AND
			|| prev->token == R_PARENTHESIS
			|| prev->token == L_PARENTHESIS
			|| prev->token == DIREC
			|| prev->token == FLE))
		return (true);
	if (is_redir_target(prev) && no_command_yet(prev->prev))
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
