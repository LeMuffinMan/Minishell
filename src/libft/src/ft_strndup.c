/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 02:21:40 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 04:10:31 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int end)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(char) * end + 1);
	if (res == NULL)
		return (NULL);
	while (i < end)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
