/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_useless_space.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:17:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 16:26:42 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "expand.h"
#include <errno.h>

void	delete_first_space(t_token **node)
{
	int		len;
	int		i;
	int		j;
	char	*res;

	i = 0;
	while ((*node)->content[0][i] && ft_isspace((*node)->content[0][i]))
		i++;
	len = 0;
	j = i;
	while ((*node)->content[0][j])
	{
		len++;
		j++;
	}
	res = ft_calloc(sizeof(char), len + 1);
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	ft_memcpy(res, &(*node)->content[0][i], len);
	free((*node)->content[0]);
	(*node)->content[0] = res;
}
