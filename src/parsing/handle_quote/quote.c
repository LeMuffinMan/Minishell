/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:01:39 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/14 20:25:48 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
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

void	parse_quote(t_token **node)
{
	t_token	*tmp;

	tmp = *node;
	if (!tmp)
		return ;
	while (tmp)
	{
		if (is_one_quote(tmp->content, '\''))
			tmp->error = PB_QUOTE;
		if (is_one_quote(tmp->content, '"'))
			tmp->error = PB_QUOTE;
		tmp = tmp->next;
	}
}
