/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:52:27 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 23:59:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	void	*tab;
	void	*zero;

	i = 0;
	if (nmemb == 0 || size == 0)
	{
		zero = malloc(0);
		if (!zero)
			return (NULL);
		return (zero);
	}
	if (size != 0 && nmemb > 2147483647 / size)
		return (NULL);
	tab = malloc(size * nmemb);
	if (tab == NULL)
		return (NULL);
	while (i < nmemb * size)
	{
		((char *)tab)[i] = '\0';
		i++;
	}
	return (tab);
}
