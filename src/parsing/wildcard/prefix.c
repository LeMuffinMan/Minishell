/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:28:27 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:55:13 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>

static void	alloc_prefix(t_token **node, char **current_dir, char **res)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]))
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				free_tab(res);
				errno = MEM_ALLOC;
				return ;
			}
			count++;
		}
		i++;
	}
	assign_wildcard(node, res, count);
}

bool	find_prefix(char *str, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[j] && src[j] != '*')
		j++;
	while (i < j)
	{
		if (src[i] != str[i])
			return (false);
		i++;
	}
	return (true);
}

void	is_prefix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	alloc_prefix(node, current_dir, res);
}
