/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:20:58 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/18 22:58:13 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_allword(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_calloc((end - start + 1), sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	return (word);
}

static size_t	ft_countword(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*str)
	{
		if (*str != c && i == 0)
		{
			i = 1;
			count++;
		}
		else if (*str == c && i != 0)
			i = 0;
		str++;
	}
	return (count);
}

static void	*ft_error_split(char **str, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	**ft_allcalloc(size_t nmemb, size_t size)
{
	char	**str;

	str = ft_calloc(nmemb, size);
	if (!str)
		return (NULL);
	else
		return (str);
}

char	**ft_split(const char *s, char c)
{
	char	**str;
	size_t	i;
	int		j;
	int		word;

	i = 0;
	j = 0;
	word = -1;
	str = ft_allcalloc((ft_countword(s, c) + 1), sizeof(char *));
	while (str != NULL && i <= ft_strlen(s))
	{
		if (s[i] != c && word < 0)
			word = i;
		else if ((s[i] == c || i == ft_strlen(s)) && word >= 0)
		{
			str[j] = ft_allword(s, word, i);
			if (!(str[j]))
				return (ft_error_split(str, j));
			word = -1;
			j++;
		}
		i++;
	}
	return (str);
}
