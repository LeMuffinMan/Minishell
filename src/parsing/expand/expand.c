/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/29 20:09:17 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "list.h"
#include "exec.h"
#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

static char	*alloc_new_expand(t_token **node, char *value, int index, int i, int j)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	char	*res;

	tmp3 = NULL;
	if ((*node)->content[i][0] == '$')
	{
		tmp1 = ft_strdup(value);
		if (!tmp1)
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'1", MEM_ALLOC);
		tmp2 = ft_strdup(&(*node)->content[i][index]);
		if (!tmp2)
		{
			free(tmp1);
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'2", MEM_ALLOC);
		}
		res = ft_strjoin(tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		if (!res)
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'3", MEM_ALLOC);
	}
	else
	{
		tmp1 = ft_strndup((*node)->content[i], index);
		if (!tmp1)
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'1", MEM_ALLOC);
		tmp2 = ft_strdup(value);
		if (!tmp2)
		{
			free(tmp1);
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'2", MEM_ALLOC);
		}
		tmp3 = ft_strjoin(tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		if (!tmp3)
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'2", MEM_ALLOC);
		tmp1 = ft_strdup(&(*node)->content[i][index + j]);
		if (!tmp1)
		{
			free(tmp3);
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'2", MEM_ALLOC);
		}
		res = ft_strjoin(tmp3, tmp1);
		free(tmp1);
		free(tmp3);
		if (!res)
			free_parse(*node, "Malloc failed in function 'alloc_new_expand'4", MEM_ALLOC);
	}
	return (res);
}

static int	export_value(char *str, char **value)
{
	int		index;
	
	index = 0;
	while (str[index])
	{
		if (str[index] == ' ' || (str[index] == '$'
			&& str[index - 1] != '\\'))
			break ;
		index++;
	}
	index++;
	*value = ft_strndup(str, index);
	#include <stdio.h>
	printf("%s\n", *value);
	return (index);
}

static int	expand_content_node(t_token **node, t_var *list_env, int i, int j)
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
	free_parse(*node, "Malloc failed in function 'expand_content_node'", MEM_ALLOC);
	res = get_value(&list_env, value);
	free(value);
	if (res)
		new_content = alloc_new_expand(node, res, index, i, j);
	free((*node)->content[i]);
	(*node)->content[i] = new_content;
	(*node)->token = EXPAND;
	return (index);
}
	
static bool	to_expand(t_token **node, t_var *list_env)
{
	int	i;
	int	j;
	
	i = 0;
	while ((*node)->content[i])
	{
		j = 0;
		while ((*node)->content[i][j])
		{
			if ((*node)->content[i][j] == '$')
			{
				if (j > 0 && (*node)->content[i][j - 1] == '\\')
					j++;
				else
					j += expand_content_node(node, list_env, i, j + 1);
			}
			j++;
		}
		i++;
	}
	return (false);
}

void	init_expand(t_token **head, t_var *list_env)
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
}
