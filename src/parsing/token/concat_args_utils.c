/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:44:43 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/09 18:27:21 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include <stdlib.h>

static void	case_of_directory_error(t_token **node)
{
	int	len;

	len = ft_strlen((*node)->content[0]);
	if ((*node)->content[0][len - 1] == '/')
		(*node)->error = IS_A_DIR;
	else
		(*node)->error = CMD_NOT_FOUND;
}

static t_token	*set_syntax_error(t_token *node)
{
	t_token	*head;
	char	*operator;
	char	*tmp;
	char	*new_content;

	operator = ft_strdup(node->content[0]);
	if (!operator)
		return (NULL);
	free_parse(node, NULL, 0);
	tmp = ft_strjoin("syntax error near unexpected token `", operator);
	free(operator);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "'");
	free(tmp);
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, NO_F_OR_D);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}

void	check_syntax_error(t_token **head)
{
	t_token	*tmp;

	if (!head | !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if ((tmp->token == PIPE && (!tmp->prev || !tmp->next))
			|| (tmp->token == O_OR && (!tmp->prev || !tmp->next))
			|| (((tmp->token == R_IN) || (tmp->token == HD)
					|| (tmp->token == TRUNC) || (tmp->token == APPEND))
				&& !tmp->content[1]))
		{
			*head = set_syntax_error(tmp);
			return ;
		}
		if (tmp->token == DIREC)
			case_of_directory_error(&tmp);
		else
			tmp = tmp->next;
	}
}
