/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokenize_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:24:25 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/13 20:10:06 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>

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

t_token	*add_new_token(char *str, int error_code)
{
	t_token	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->token = NO_TOKEN;
	new_node->error = error_code;
	new_node->content = alloc_tab(str);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->priority = 10;
	new_node->seq = true;
	new_node->group = NULL;
	return (new_node);
}

void	add_back(t_token **head, char *str)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = add_new_token(str, SUCCESS);
	if (!new_node)
	{
		free_parse(*head, NULL, 0);
		return ;
	}
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->prev = tmp;
}

void	free_parse(t_token *list, const char *str, int error)
{
	t_token	*next_node;

	if (!list)
		return ;
	next_node = NULL;
	if (list->prev)
	{
		while (list->prev)
			list = list->prev;
	}
	while (list)
	{
		if (list->next)
			next_node = list->next;
		if (list->group)
			free_parse(list->group, NULL, SUCCESS);
		if (list->content)
		free_tab(list->content);
		free(list);
		list = next_node;
	}
	if (str)
		ft_error(str, error);
}
