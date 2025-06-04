/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:26:19 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/18 22:58:48 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*tmp;

	tmp = (char *)s;
	i = ft_strlen(tmp);
	while (i > 0)
	{
		if (tmp[i] == (char)c)
			return (&tmp[i]);
		else
			i--;
	}
	if (tmp[i] == (char)c)
		return (&tmp[i]);
	return (NULL);
}
