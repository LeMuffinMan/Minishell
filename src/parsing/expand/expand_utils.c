/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:47:42 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 17:38:57 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"
#include "list.h"
#include <stdlib.h>

void	copy_value(t_token *node, char *value, char **value_cpy)
{
	if (!value)
		*value_cpy = ft_calloc(1, sizeof(char));
	else
		*value_cpy = ft_strdup(value);
	if (!*value_cpy)
		free_parse(node,
			"Malloc failed in function 'copy_value'", MEM_ALLOC);
}

char	*alloc_first_expand(char *value, char *str_to_expand, int index)
{
	char	*next;
	char	*res;

	next = ft_strdup(&str_to_expand[index]);
	if (!next)
	{
		free(value);
		return (NULL);
	}
	res = ft_strjoin(value, next);
	free(value);
	free(next);
	if (!res)
		return (NULL);
	return (res);
}

static char	*alloc_first_part(char *str_node, char *value, int j)
{
	char	*expand_value;
	char	*res;
	char	*prev;

	prev = ft_strndup(str_node, j);
	if (!prev)
		return (NULL);
	expand_value = ft_strdup(value);
	if (!expand_value)
	{
		free(prev);
		return (NULL);
	}
	res = ft_strjoin(prev, expand_value);
	free(prev);
	free(expand_value);
	return (res);
}

char	*ft_strjoin3(char *str_node, char *value, int j, int index)
{
	char	*first_part;
	char	*last_part;
	char	*res;

	first_part = alloc_first_part(str_node, value, j);
	if (!first_part)
		return (NULL);
	last_part = ft_strdup(&str_node[index + j + 1]);
	if (!last_part)
	{
		free(first_part);
		return (NULL);
	}
	res = ft_strjoin(first_part, last_part);
	free(last_part);
	free(first_part);
	if (!res)
		return (NULL);
	return (res);
}
