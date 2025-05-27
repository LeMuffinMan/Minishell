/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:46:52 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:57:26 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>

bool	is_to_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

bool	del_last_quote(t_token **node)
{
	int		len;
	char	*end;

	len = ft_strlen((*node)->content[0]) - 1;
	if ((*node)->content[0][len] == '"' || (*node)->content[0][len] == '\'')
	{
		end = ft_strndup((*node)->content[0], len);
		if (!end)
		{
			errno = MEM_ALLOC;
			free_parse(*node, NULL, MEM_ALLOC);
			return (false);
		}
		free((*node)->content[0]);
		(*node)->content[0] = end;
	}
	return (true);
}
