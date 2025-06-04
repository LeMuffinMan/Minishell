/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:10:09 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "list.h"
#include "env_utils.h"
#include "libft.h"
#include <stdlib.h>
#include <errno.h>

static char	*alloc_new_expand(t_token **node, char *value,
		int index, int j)
{
	char	*value_cpy;
	char	*res;

	copy_value(*node, value, &value_cpy);
	if ((*node)->content[0][0] == '$')
	{
		res = alloc_first_expand(value_cpy, (*node)->content[0], index + 1);
		if (!res)
		{
			free_parse(*node);
			errno = MEM_ALLOC;
		}
		return (res);
	}
	if (value)
		res = ft_strjoin3((*node)->content[0], value, j, index);
	else
		res = ft_strjoin3((*node)->content[0], value_cpy, j, index);
	free(value_cpy);
	if (!res)
	{
		free_parse(*node);
		errno = MEM_ALLOC;
	}
	return (res);
}

static int	export_value(char *str, char **value)
{
	int		index;

	index = 0;
	while (str[index])
	{
		if (ft_isspace(str[index])
			|| (index > 0
				&& ((str[index] == '$' && str[index - 1] != '\\')
					|| str[index - 1] == '?' || str[index] == '*'))
			|| str[index] == '/'
			|| str[index] == '\'')
			break ;
		index++;
	}
	*value = ft_strndup(str, index);
	if (!*value)
		errno = MEM_ALLOC;
	return (index);
}

void	expand_node_content(t_token **node,
							t_var *list_env, int j, t_lists *lists)
{
	char	*res;
	char	*value;
	char	*new_content;
	int		index;

	index = export_value(&(*node)->content[0][j], &value);
	if (errno == MEM_ALLOC)
	{
		free_parse(*node);
		errno = MEM_ALLOC;
		return ;
	}
	if (!ft_strncmp((*node)->content[0], "$?", 2))
		res = ft_itoa(lists->exit_code);
	else
		res = get_value(&list_env, value);
	free(value);
	new_content = alloc_new_expand(node, res, index, j - 1);
	if (!ft_strcmp((*node)->content[0], "$?"))
		free(res);
	free((*node)->content[0]);
	(*node)->content[0] = new_content;
	(*node)->token = EXPAND;
	if ((*node)->error != LITERAL_EXPAND)
		(*node)->error = SUCCESS;
}
