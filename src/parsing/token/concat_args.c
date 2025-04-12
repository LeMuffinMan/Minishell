/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 04:50:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/11 17:14:00 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include <stdlib.h>

static char	**cpy_tab(char **dest, char **src, int index)
{
	int		i;
	int		len;

	len = ft_tab_len(src);
	i = 0;
	while (i < len)
	{
		dest[index] = ft_strdup(src[i]);
		if (!dest[index])
			return (NULL);
		index++;
		i++;
	}
	return (dest);
}

static bool	is_same_family(t_token *node)
{
	if ((node->token == CMD) || node->token == APPEND
		|| (node->token == BUILT_IN) || (node->token == HD)
		|| (node->token == R_IN) || (node->token == TRUNC))
	{
		if (node->next)
		{
			if ((node->next->error != 0) || (node->next->token == D_QUOTE)
				|| (node->next->token == S_QUOTE))
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
	/* int		i; */

	len_old = ft_tab_len(old);
	len_new = ft_tab_len(new);
	/* i = 0; */
	res = malloc(sizeof(char *) * (len_old + len_new + 1));
	if (!res)
		free_parse(node,
			"Malloc failed in function 'join_node_content'", MEM_ALLOC);
	res = cpy_tab(res, old, 0);
	if (!res)
		free_parse(node, "Malloc failed in function 'cpy_tab'", MEM_ALLOC);
	res = cpy_tab(res, new, len_old);
	if (!res)
		free_parse(node, "Malloc failed in function 'cpy_tab'", MEM_ALLOC);
	res[len_new + len_old] = NULL;
	return (res);
}

static void	change_node(t_token **node)
{
	t_token	*next_node;
	/* int		i; */
	char	**new_content;

	next_node = (*node)->next;
	if (!next_node)
		return ;
	/* i = 0; */
	new_content = join_node_content(*node,
			(*node)->content, next_node->content);
	if (!new_content)
		free_parse(*node, "MAlloc failed in function 'change_node'", MEM_ALLOC);
	free_tab((*node)->content);
	(*node)->content = new_content;
	(*node)->next = next_node->next;
	free_tab(next_node->content);
	free(next_node);
	next_node = NULL;
}

void	concat_args(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	if (!tmp)
		return ;
	while (tmp)
	{
		if (is_same_family(tmp))
			change_node(&tmp);
		tmp = tmp->next;
	}
	if (tmp && is_same_family(tmp))
		change_node(&tmp);
}
