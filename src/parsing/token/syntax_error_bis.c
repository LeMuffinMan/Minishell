/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:20:00 by oelleaum          #+#    #+#             */
/*   Updated: 2026/09/17 19:20:00 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"

/*
** An operator that lost its operand is not itself the unexpected token:
** bash names what came instead. `whoami >` ends the line, so the offender
** is `newline'; in `echo > <` it is the `<' that showed up where a file
** name was due.
*/
static char	*offending_name(t_token *node)
{
	t_token	*next;

	next = node->next;
	while (next && next->token == SPACE)
		next = next->next;
	if (!next || !next->content || !next->content[0])
		return (ft_strdup("newline"));
	return (ft_strdup(next->content[0]));
}

t_token	*set_redir_syntax_error(t_token *node)
{
	t_token	*head;
	char	*name;
	char	*tmp;
	char	*new_content;

	if (!node || !node->content || !node->content[0])
		return (NULL);
	name = offending_name(node);
	if (!name)
		return (NULL);
	free_parse(node, NULL, 0);
	tmp = ft_strjoin("syntax error near unexpected token `", name);
	free(name);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "'");
	free(tmp);
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, NO_F_OR_D, false);
	free(new_content);
	return (head);
}
