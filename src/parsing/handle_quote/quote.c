/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:01:39 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/02 23:55:14 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"

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

static void	del_quote_char(t_token **node)
{
	char	*start;
	char	*end;
	int		len;

	if ((*node)->content[0][0] == '"' || (*node)->content[0][0] == '\'')
	{
		start = ft_strdup(&(*node)->content[0][1]);
		if (!start)
			free_parse(*node,
				"Malloc failed in function del_quote_char", MEM_ALLOC);
		free((*node)->content[0]);
		(*node)->content[0] = start;
	}
	len = ft_strlen((*node)->content[0]) - 1;
	if ((*node)->content[0][len] == '"' || (*node)->content[0][len] == '\'')
	{
		end = ft_strndup((*node)->content[0], len);
		if (!end)
			free_parse(*node,
				"Malloc failed in function del_quote_char", MEM_ALLOC);
		free((*node)->content[0]);
		(*node)->content[0] = end;
	}
}

void	parse_quote(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	if (!tmp)
		return ;
	while (tmp)
	{
		if (is_one_quote(tmp->content, '\''))
			tmp->error = PB_QUOTE;
		if (is_one_quote(tmp->content, '"'))
			tmp->error = PB_QUOTE;
		del_quote_char(&tmp);
		tmp = tmp->next;
	}
}
