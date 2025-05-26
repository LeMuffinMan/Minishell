/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:28:24 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:56:05 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "token.h"
#include "libft.h"
#include "list.h"
#include <errno.h>
#include <stdlib.h>

static void	alloc_joker(t_token **node, char **current_dir, char **res)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (current_dir[i][0] != '.')
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				free_tab(current_dir);
				errno = MEM_ALLOC;
				return ;
			}
			count++;
		}
		i++;
	}
	assign_wildcard(node, res, count);
}

void	is_joker(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (current_dir[i][0] != '.')
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	alloc_joker(node, current_dir, res);
}
