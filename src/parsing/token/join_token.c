/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:26:02 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 16:13:41 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "stdlib.h"
#include "libft.h"
#include "expand.h"
#include <errno.h>

static bool	token_valid_for_join(t_token *node)
{
	if (node->token == O_AND || node->token == O_OR
		|| node->token == R_IN || node->token == HD || node->token == APPEND
		|| node->token == TRUNC || node->token == PIPE
		|| node->token == L_PARENTHESIS || node->token == R_PARENTHESIS
		|| (node->content[0] && node->content[0][0]
		&& ft_isspace(node->content[0][0])))
		return (false);
	else
		return (true);
}

char	**join_content(t_token *node, char **old, char **new)
{
	char	**res;
	int		len_old;
	int		len_new;

	len_old = ft_strlen(old[0]);
	len_new = ft_strlen(new[0]);
	res = malloc(sizeof(char *) * 2);
	if (!res)
	{
		free_parse(node, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return (NULL);
	}
	res[0] = malloc(sizeof(char) * (len_old + len_new + 1));
	if (!res[0])
	{
		free_parse(node, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return (NULL);
	}
	ft_memcpy(res[0], old[0], len_old);
	ft_memcpy(&res[0][len_old], new[0], len_new);
	res[0][len_old + len_new] = '\0';
	res[1] = NULL;
	return (res);
}

bool	join_token(t_token **head)
{
	t_token	*tmp;
	bool	flag;

	flag = false;
	if (!head || !*head)
		return (flag);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == SPACE)
			tmp = tmp->next;
		else if (token_valid_for_join(tmp)
			&& tmp->next && tmp->next->token != SPACE
			&& token_valid_for_join(tmp->next))
		{
			handle_change_node(&tmp, false);
			if (tmp->prev)
				tmp->token = ARG;
			flag = true;
		}
		else
			tmp = tmp->next;
	}
	handle_space(head);
	return (flag);
}
