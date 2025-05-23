/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:01:39 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 23:21:27 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include <errno.h>

static bool	case_only_quote(t_token **node)
{
	char	*res;

	res = NULL;
	if (((*node)->content[0][0] == '\'' && (*node)->content[0][1] == '\'')
		|| ((*node)->content[0][0] == '"' && (*node)->content[0][1] == '"'))
	{
		res = ft_strdup("");
		if (!res)
			free_parse(*node,
				"Malloc failed in function 'case_only_quote'", MEM_ALLOC);
		free((*node)->content[0]);
		(*node)->content[0] = res;
		return (true);
	}
	return (false);
}

static bool	is_one_quote(char **content, char c)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (content[i])
	{
		j = 0;
		while (content[i][j])
		{
			if (content[i][j] == c)
				count++;
			j++;
			if (content[i][j] == c && content[i][j - 1] == '\\')
				j++;
		}
		if (count % 2 != 0)
			return (true);
		i++;
	}
	return (false);
}

static bool	del_quote_char(t_token **node)
{
	char	*start;

	if (case_only_quote(node))
		return (true);
	if ((*node)->content[0][0] == '"' || (*node)->content[0][0] == '\'')
	{
		start = ft_strdup(&(*node)->content[0][1]);
		if (!start)
		{
			errno = MEM_ALLOC;
			free_parse(*node, NULL, MEM_ALLOC);
			return (false);
		}
		free((*node)->content[0]);
		(*node)->content[0] = start;
	}
	if (!del_last_quote(node))
		return (false);
	return (true);
}

static bool	parse_quote(t_token **node)
{
	if (is_one_quote((*node)->content, '\''))
		(*node)->error = PB_QUOTE;
	if (is_one_quote((*node)->content, '"'))
		(*node)->error = PB_QUOTE;
	if (((*node)->token == D_QUOTE || (*node)->token == D_QUOTE
			|| (*node)->error == QUOTE)
		&& is_to_expand((*node)->content[0]))
		(*node)->error = LITERAL_EXPAND;
	if ((*node)->error != PB_QUOTE)
	{
		if (!del_quote_char(node))
			return (false);
	}
	return (true);
}

bool	is_quote(t_token **node)
{
	int		len;
	bool	flag;

	len = ft_strlen((*node)->content[0]);
	flag = false;
	if ((*node)->content[0][0] == '"')
	{
		(*node)->token = D_QUOTE;
		(*node)->error = QUOTE;
		flag = true;
	}
	else if ((*node)->content[0][0] == '\'')
	{
		(*node)->token = S_QUOTE;
		(*node)->error = QUOTE;
		flag = true;
	}
	if (flag)
	{
		if (!parse_quote(node))
			return (false);
		return (true);
	}
	return (false);
}
