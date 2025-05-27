/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parenthesis_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:07:28 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 03:13:15 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"
#include "token.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>

int	find_len_new_content(t_token *node, t_token *end)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (node)
	{
		i = -1;
		while (node->content[++i])
			len += ft_strlen(node->content[i]) + 1;
		if (node == end)
			break ;
		node = node->next;
	}
	return (len);
}

void	copy_nodes_content(t_token *start, t_token *end, char **res,
				t_type current_token)
{
	int		pos;
	int		i;
	int		word_len;

	pos = 0;
	while (start)
	{
		i = -1;
		while (start->content[++i])
		{
			if (current_token == start->token
				|| start->token == R_PARENTHESIS)
				break ;
			word_len = ft_strlen(start->content[i]);
			ft_memcpy(*res + pos, start->content[i], word_len);
			pos += word_len;
			(*res)[pos++] = ' ';
		}
		if (start == end)
			break ;
		start = start->next;
	}
	if (pos > 0 && (*res)[pos - 1] == ' ')
		(*res)[pos - 1] = '\0';
}

void	free_node_unused(t_token *node)
{
	t_token	*next_node;

	if (node->next)
		next_node = node->next;
	while (node)
	{
		next_node = node->next;
		if (node->content)
			free_tab(node->content);
		if (node)
			free(node);
		node = NULL;
		node = next_node;
	}
}
