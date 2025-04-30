/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/30 23:33:32 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "list.h"
#include "exec.h"
#include "libft.h"
#include <stdlib.h>

static char	*alloc_new_expand(t_token **node, char *value,
		int index, int i, int j)
{
	char	*value_cpy;
	char	*res;

	copy_value(*node, value, &value_cpy);
	if ((*node)->content[i][0] == '$')
	{
		res = alloc_first_expand(value_cpy, (*node)->content[i], index + 1);
		free(value_cpy);
		if (!res)
			free_parse(*node, "Malloc failed in expand'", MEM_ALLOC);
	}
	else
	{
		if (value)
			res = ft_strjoin3((*node)->content[i], value, j, index);
		else
		{
			res = ft_strjoin3((*node)->content[i], value_cpy, j, index);
			free(value_cpy);
		}
		if (!res)
			free_parse(*node, "Malloc failed in expand'", MEM_ALLOC);
	}
	return (res);
}

static int	export_value(char *str, char **value)
{
	int		index;

	index = 0;
	while (str[index])
	{
		if (ft_isspace(str[index]) || (str[index] == '$'
				&& str[index - 1] != '\\'))
			break ;
		index++;
	}
	index++;
	*value = ft_strndup(str, index);
	return (index - 1);
}

static int	expand_node_content(t_token **node, t_var *list_env, int i, int j)
{
	char	*res;
	char	*value;
	char	*new_content;
	int		index;

	res = NULL;
	index = 0;
	value = NULL;
	index = export_value(&(*node)->content[i][j], &value);
	if (!value)
		free_parse(*node,
			"Malloc failed in function 'expand_node_content'", MEM_ALLOC);
	res = get_value(&list_env, value);
	free(value);
	new_content = alloc_new_expand(node, res, index, i, j - 1);
	free((*node)->content[i]);
	(*node)->content[i] = new_content;
	(*node)->token = EXPAND;
	return (index);
}

void	to_expand(t_token **node, t_var *list_env)
{
	int	i;
	int	j;

	i = 0;
	while ((*node)->content[i])
	{
		j = 0;
		while ((*node)->content[i] && (*node)->content[i][j])
		{
			if ((*node)->content[i][j] == '$')
			{
				if (j > 0 && (*node)->content[i][j - 1] == '\\')
					j++;
				else
					j += expand_node_content(node, list_env, i, j + 1);
			}
			j++;
		}
		i++;
	}
}

/* void	init_expand(t_token **head, t_var *list_env)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (tmp->error != 0 || tmp->token == D_QUOTE)
			to_expand(&tmp, list_env);
		tmp = tmp->next;
	}
} */
