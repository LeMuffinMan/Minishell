/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:44:43 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 17:35:29 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include <errno.h>
#include <stdlib.h>

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


