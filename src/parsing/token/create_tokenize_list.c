/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokenize_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:24:25 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/10 20:51:46 by asinsard         ###   ########lyon.fr   */
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

static t_token	*add_new_node(char *str)
{
	t_token	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->token = -1;
	new_node->error = SUCCESS;
	new_node->content = alloc_tab(str);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

void	add_back(t_token **head, char *str)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = add_new_node(str);
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
	t_token	*tmp;
	t_token	*next_node;

	if (!list)
		return ;
	if (list->prev)
	{
		while (list->prev)
			list = list->prev;
	}
	tmp = list;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->content)
			free_tab(tmp->content);
		free(tmp);
		tmp = next_node;
	}
	if (str)
		ft_error(str, error);
}
