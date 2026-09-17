/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:45:00 by oelleaum          #+#    #+#             */
/*   Updated: 2026/09/17 19:45:00 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include <stdlib.h>
#include "libft.h"

static bool	is_cmd_boundary(t_type token)
{
	return (token == PIPE || token == O_AND || token == O_OR
		|| token == L_PARENTHESIS || token == R_PARENTHESIS);
}

static bool	is_redirection(t_type token)
{
	return (token == R_IN || token == TRUNC
		|| token == APPEND || token == HD);
}

// The command this word belongs to, or NULL when nothing before it in the
// same simple command is one.
static t_token	*owning_command(t_token *node)
{
	while (node)
	{
		if (is_cmd_boundary(node->token))
			return (NULL);
		if (node->token == CMD || node->token == BUILT_IN)
			return (node);
		node = node->prev;
	}
	return (NULL);
}

// A builtin gets its arguments as one concatenated string, so each of them
// carries its own trailing space as the separator; an external command gets
// a real argv and stores them bare. The newcomer follows the same rule as
// the arguments already there.
static void	attach_one(t_token **head, t_token *node, t_token *cmd)
{
	int		last;
	char	*spaced;

	last = ft_tab_len(cmd->content) - 1;
	if (cmd->token == BUILT_IN && last >= 1 && cmd->content[last][0]
		&& cmd->content[last][ft_strlen(cmd->content[last]) - 1] != ' ')
	{
		spaced = ft_strjoin(cmd->content[last], " ");
		if (!spaced)
			return ;
		free(cmd->content[last]);
		cmd->content[last] = spaced;
	}
	if (node->prev)
		node->prev->next = node->next;
	else
		*head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	node->prev = cmd;
	node->next = cmd->next;
	if (cmd->next)
		cmd->next->prev = node;
	cmd->next = node;
	change_node(&cmd, true);
}

/*
** `ls > out extra` is the same command as `ls extra > out`: a redirection
** may sit anywhere inside a simple command, and the words around it stay
** arguments of it. concat_args already gathered everything written before
** the redirection, so what is left after it is moved back onto its command.
*/
void	attach_args_after_redir(t_token **head)
{
	t_token	*node;
	t_token	*next;
	t_token	*cmd;

	if (!head || !*head)
		return ;
	node = *head;
	while (node)
	{
		next = node->next;
		if (node->prev && is_redirection(node->prev->token)
			&& !is_redirection(node->token) && !is_cmd_boundary(node->token)
			&& node->token != CMD && node->token != BUILT_IN
			&& node->token != SPACE)
		{
			cmd = owning_command(node->prev);
			if (cmd)
				attach_one(head, node, cmd);
		}
		node = next;
	}
}
