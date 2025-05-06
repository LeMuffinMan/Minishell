/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/02 23:54:50 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "list.h"
#include "exec.h"
#include "libft.h"
#include <stdlib.h>

static char	*alloc_new_expand(t_token **node, char *value,
		int index, int j)
{
	char	*value_cpy;
	char	*res;

	copy_value(*node, value, &value_cpy);
	if ((*node)->content[0][0] == '$')
	{
		res = alloc_first_expand(value_cpy, (*node)->content[0], index + 1);
		free(value_cpy);
		if (!res)
			free_parse(*node, "Malloc failed in expand'", MEM_ALLOC);
	}
	else
	{
		if (value)
			res = ft_strjoin3((*node)->content[0], value, j, index);
		else
		{
			res = ft_strjoin3((*node)->content[0], value_cpy, j, index);
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
		if (ft_isspace(str[index]) || (index > 0
				&& str[index] == '$' && str[index - 1] != '\\'))
			break ;
		index++;
	}
	*value = ft_strndup(str, index);
	return (index);
}

static int	expand_node_content(t_token **node, t_var *list_env, int j)
{
	char	*res;
	char	*value;
	char	*new_content;
	int		index;

	res = NULL;
	index = 0;
	value = NULL;
	index = export_value(&(*node)->content[0][j], &value);
	if (!value)
		free_parse(*node,
			"Malloc failed in function 'expand_node_content'", MEM_ALLOC);
	res = get_value(&list_env, value);
	free(value);
	new_content = alloc_new_expand(node, res, index, j - 1);
	free((*node)->content[0]);
	(*node)->content[0] = new_content;
	(*node)->token = EXPAND;
	(*node)->error = SUCCESS;
	return (index);
}

void	to_expand(t_token **node, t_var *list_env)
{
	int	j;

	j = 0;
	while ((*node)->content[0][j])
	{
		if ((*node)->content[0][j] == '$')
		{
			if ((j > 0 && (*node)->content[0][j - 1] == '\\')
				|| ((*node)->content[0][j + 1]
				&& ft_isspace((*node)->content[0][j + 1])))
				j++;
			else
				j += expand_node_content(node, list_env, j + 1);
		}
		j++;
	}
}

bool	init_expand(t_token **head, t_var *list_env)
{
	t_token	*tmp;
	bool	flag;

	if (!head || !*head)
		return (false);
	tmp = *head;
	flag = false;
	while (tmp)
	{
		if (tmp->error != 0 || tmp->token == D_QUOTE)
		{
			to_expand(&tmp, list_env);
			flag = true;
		}
		tmp = tmp->next;
	}
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == EXPAND && (tmp->next && tmp->next->token == SPACE)
			&& (tmp->next->next && tmp->next->next->token == EXPAND))
			add_space(&tmp);
		tmp = tmp->next;
	}
	return (flag);
}
