/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:26:36 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 00:26:13 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*tmp1;
	char	*tmp2;
	size_t	i;

	tmp1 = (char *)src;
	tmp2 = dest;
	i = 0;
	if (tmp1 == NULL && tmp2 == NULL)
		return (dest);
	while (i < n)
	{
		tmp2[i] = tmp1[i];
		i++;
	}
	return (dest);
}
