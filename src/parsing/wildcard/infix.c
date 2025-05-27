/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infix.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:28:20 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:55:56 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>

static void	alloc_infix(t_token **node, char **current_dir,
						char **res, int suffix)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0])
			&& find_suffix(current_dir[i], &(*node)->content[0][suffix + 1]))
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

void	is_infix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;
	int		suffix;

	i = 0;
	suffix = 0;
	count = 0;
	while ((*node)->content[0][suffix] && (*node)->content[0][suffix] != '*')
		suffix++;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0])
			&& find_suffix(current_dir[i], &(*node)->content[0][suffix + 1]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	alloc_infix(node, current_dir, res, suffix);
}
