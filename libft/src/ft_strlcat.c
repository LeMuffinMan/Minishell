/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:26:18 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 00:26:02 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	lend;
	size_t	lens;
	size_t	i;

	lend = ft_strlen((const char *)dest);
	lens = ft_strlen(src);
	i = 0;
	if (size <= lend)
		return (size + lens);
	while (src[i] && i < size - lend - 1)
	{
		dest[lend + i] = src[i];
		i++;
	}
	dest[lend + i] = '\0';
	return (lend + lens);
}
