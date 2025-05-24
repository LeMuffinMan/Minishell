/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:26:02 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 00:57:01 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "stdlib.h"
#include "libft.h"
#include "expand.h"
#include <errno.h>

static bool	token_valid_for_join(t_type token)
{
	if (token == O_AND || token == O_OR
		|| token == R_IN || token == HD || token == APPEND
		|| token == TRUNC || token == PIPE
		|| token == L_PARENTHESIS || token == R_PARENTHESIS)
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
		else if (token_valid_for_join(tmp->token)
			&& tmp->next && tmp->next->token != SPACE)
		{
			change_node(&tmp, false);
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
