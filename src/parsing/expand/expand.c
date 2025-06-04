/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/04 18:13:59 by asinsard         ###   ########lyon.fr   */
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
			free_parse(*node, NULL, MEM_ALLOC);
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
		free_parse(*node, NULL, MEM_ALLOC);
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

static void	expand_node_content(t_token **node,
								t_var *list_env, int j, t_lists *lists)
{
	char	*res;
	char	*value;
	char	*new_content;
	int		index;

	index = export_value(&(*node)->content[0][j], &value);
	if (errno == MEM_ALLOC)
	{
		free_parse(*node, NULL, MEM_ALLOC);
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

static bool	to_expand(t_token **node, t_var *list_env, t_lists *lists)
{
	int		j;
	bool	flag;

	j = -1;
	flag = false;
	while ((*node)->content[0][++j] && errno != MEM_ALLOC)
	{
		if ((*node)->content[0][j] == '$' && ((*node)->content[0][j + 1]
			|| ((*node)->next && ((*node)->next->token == S_QUOTE
			|| (*node)->next->token == D_QUOTE))))
		{
			if ((j > 0 && (*node)->content[0][j - 1] == '\\')
			|| ((*node)->content[0][j + 1]
			&& ft_isspace((*node)->content[0][j + 1])))
				j++;
			else
			{
				expand_node_content(node, list_env, j + 1, lists);
				if ((*node)->content[0][0])
					flag = true;
				j = -1;
			}
		}
	}
	return (flag);
}

bool	init_expand(t_token **head, t_var *list_env, t_lists *lists)
{
	t_token	*tmp;
	bool	flag;

	tmp = *head;
	flag = false;
	while (tmp)
	{
		if (tmp->token == S_QUOTE || (tmp->prev
			&& tmp->prev->token == HD)
			|| (tmp->prev && tmp->prev->prev
			&& tmp->prev->prev->token == HD))
			tmp = tmp->next;
		else
		{
			flag = to_expand(&tmp, list_env, lists);
			if (errno == MEM_ALLOC)
				return (false);
			tmp = tmp->next;
		}
	}
	handle_space(head);
	return (flag);
}
