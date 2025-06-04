/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokenize_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:24:25 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:12:37 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static char	**alloc_tab(char *str)
{
	char	**res;

	res = malloc(sizeof(char *) * 2);
	if (!res)
		return (NULL);
	res[0] = ft_strdup(str);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	res[1] = NULL;
	return (res);
}

static bool	alloc_token_content(t_token **new_node, char *str, bool flag)
{
	(*new_node)->content = alloc_tab(str);
	if (!(*new_node)->content)
	{
		errno = MEM_ALLOC;
		return (false);
	}
	errno = SUCCESS;
	if (flag)
	{
		(*new_node)->curr_dir = getcwd(NULL, 0);
		if (!(*new_node)->curr_dir)
		{
			if (errno != ENOENT)
			{
				free_tab((*new_node)->content);
				(*new_node)->content = NULL;
				errno = MEM_ALLOC;
				return (false);
			}
		}
	}
	else
		(*new_node)->curr_dir = NULL;
	return (true);
}

t_token	*add_new_token(char *str, int error_code, bool flag)
{
	t_token	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
	{
		errno = MEM_ALLOC;
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->token = NO_TOKEN;
	new_node->error = error_code;
	if (!alloc_token_content(&new_node, str, flag))
	{
		free(new_node);
		errno = MEM_ALLOC;
		return (NULL);
	}
	new_node->priority = 10;
	new_node->group = NULL;
	return (new_node);
}

bool	add_back(t_token **head, char *str)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = add_new_token(str, SUCCESS, true);
	if (!new_node)
	{
		free_parse(*head);
		errno = MEM_ALLOC;
		return (false);
	}
	if (!*head)
	{
		*head = new_node;
		return (true);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->prev = tmp;
	return (true);
}

void	free_parse(t_token *list)
{
	t_token	*next_node;
	t_token	*group;

	if (!list)
		return ;
	while (list->prev)
		list = list->prev;
	while (list)
	{
		next_node = list->next;
		group = list->group;
		if (list->content)
			free_tab(list->content);
		list->next = NULL;
		list->prev = NULL;
		list->group = NULL;
		if (list->curr_dir)
			free(list->curr_dir);
		free(list);
		if (group)
			free_parse(group);
		list = next_node;
	}
}
