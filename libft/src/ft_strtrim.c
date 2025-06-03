/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:10:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/18 22:58:51 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_set(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	unsigned int	start;
	unsigned int	end;
	unsigned int	i;

	start = 0;
	end = 0;
	i = 0;
	while (check_set(set, s1[i]) && s1[i])
	{
		i++;
		start++;
	}
	while (s1[i])
	{
		end++;
		if (!check_set(set, s1[i]))
			end = 0;
		i++;
	}
	return (ft_substr(s1, start, (ft_strlen(&s1[start]) - end)));
}
