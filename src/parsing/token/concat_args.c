/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 04:50:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/02 23:48:56 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include "expand.h"
#include "structs.h"
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
		{
			free_tab(dest);
			return (NULL);
		}
		index++;
		i++;
	}
	return (dest);
}

static bool	is_same_family(t_token *node)
{
	if ((node->token == APPEND)
		|| (node->token == HD)
		|| (node->token == R_IN) || (node->token == TRUNC))
	{
		if (node->next && node->next->token != APPEND
			&& node->next->token != HD
			&& node->next->token != R_IN && node->next->token != TRUNC)
			return (true);
	}
	else if ((node->token == BUILT_IN) || (node->token == CMD))
	{
		if (node->next)
		{
			if ((node->next->error != 0) || (node->next->token == CMD)
				|| (node->next->token == D_QUOTE)
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

	len_old = ft_tab_len(old);
	len_new = ft_tab_len(new);
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
	char	**new_content;

	next_node = (*node)->next;
	if (!next_node)
		return ;
	new_content = join_node_content(*node,
			(*node)->content, next_node->content);
	if (!new_content)
		free_parse(*node, "Malloc failed in function 'change_node'", MEM_ALLOC);
	free_tab((*node)->content);
	(*node)->content = new_content;
	(*node)->next = next_node->next;
	if ((*node)->next)
		(*node)->next->prev = *node;
	free_tab(next_node->content);
	free(next_node);
	if (((*node)->token == APPEND)
		|| ((*node)->token == HD)
		|| ((*node)->token == R_IN) || ((*node)->token == TRUNC))
		*node = (*node)->next;
}

void	concat_args(t_token **head, t_var *list_env, char **envp)
{
	t_token	*tmp;

	if (!*head)
		return ;
	parse_quote(head);
	if (init_expand(head, list_env))
		assign_token(head, envp, list_env, true);
	delete_space_node(head);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == D_QUOTE || tmp->token == S_QUOTE)
			tmp->error = SUCCESS;
		if (is_same_family(tmp))
			change_node(&tmp);
		else
			tmp = tmp->next;
	}
	check_syntax_error(head);
}
