/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:14:44 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 03:15:49 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "token.h"
#include <errno.h>

char	**copy_tab(t_token *node, char **dest, char **src, int index)
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
			free_parse(node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
			return (NULL);
		}
		index++;
		i++;
	}
	return (dest);
}
