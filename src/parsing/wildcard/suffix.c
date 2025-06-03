/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suffix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:28:30 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 19:20:45 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "token.h"
#include "libft.h"
#include "list.h"
#include <errno.h>
#include <stdlib.h>

static void	alloc_suffix(t_token **node, char **current_dir, char **res)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_suffix(current_dir[i], &(*node)->content[0][1]))
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				errno = MEM_ALLOC;
				free_tab(res);
				return ;
			}
			count++;
		}
		i++;
	}
	assign_wildcard(node, res, count);
}

static int	len_without_joker(char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			len++;
		i++;
	}
	return (len);
}

bool	find_suffix(char *str, char *src)
{
	int		i;
	int		j;
	int		suffix;
	int		len;

	i = 0;
	j = 0;
	len = len_without_joker(src);
	while (str[i])
	{
		j = 0;
		suffix = i;
		while (src[j] && (src[j + 1] && src[j + 1] != '*'))
		{
			if (src[j] != str[i])
				break ;
			j++;
			suffix++;
		}
		if (suffix == len)
			return (true);
		i++;
	}
	return (false);
}


	// while (str[i])
	// 	i++;
	// while (src[j] && (src[j + 1] && src[j + 1] != '*'))
	// 	j++;
	// while (j > 0)
	// {
	// 	if (src[j] != str[i])
	// 		return (false);
	// 	i--;
	// 	j--;
	// }
void	is_suffix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;

	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_suffix(current_dir[i], &(*node)->content[0][1]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	alloc_suffix(node, current_dir, res);
}
