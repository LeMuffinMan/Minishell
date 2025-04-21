/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:40:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/18 22:58:42 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, int size)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	if (size <= 0)
		return (-1);
	while (i < size && (str1[i] || str2[i]))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}
