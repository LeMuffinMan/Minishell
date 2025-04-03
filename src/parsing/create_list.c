/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:24:25 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 02:02:42 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>

char	**alloc_tab(char *str)
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

t_parse	*add_new_node(char *str)
{
	t_parse	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_parse));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->token = -1;
	new_node->data = malloc(sizeof(t_data));
	if (!new_node->data)
	{
		free(new_node);
		return (NULL);
	}
	new_node->data->content = alloc_tab(str);
	if (!new_node->data->content)
	{
		free(new_node->data);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

void	add_back(t_parse **head, char *str)
{
	t_parse	*new_node;
	t_parse	*tmp;

	new_node = add_new_node(str);
	if (!new_node)
	{
		free_parse(*head);
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

void	free_parse(t_parse *stack)
{
	t_parse	*tmp;
	t_parse	*next_node;

	if (!stack)
		return ;
	if (stack->prev)
	{
		while (stack->prev)
			stack = stack->prev;
	}
	tmp = stack;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->data->content)
			free_tab(tmp->data->content);
		if (tmp->data)
			free(tmp->data);
		free(tmp);
		tmp = next_node;
	}
}

void	display_list(t_parse *head)
{
	t_parse	*tmp;
	int		i;
	char	*tab[12] = {"Append", "CMD", "Double quote", "Here_doc", "Limiter", "And", "Or", "Pipe", "Redirection infile", "Simple quote", "Truncate", "Wildcard"};

	i = 1;
	tmp = head;
	if (!tmp)
		ft_error("list doesn't exist", EXIT_FAILURE);
	ft_printf("%sNULL\n ^\n |\n%s", BOLD_BLUE, STOP_COLOR);
	while (tmp->next)
	{
		if ((int)tmp->token >= 0)
			ft_printf("%sNODE %d: [%s%s%s]--->[%s%s%s]\n | ^\n v |\n%s", BOLD_BLUE, i, BOLD_GREEN, tmp->data->content[0], BOLD_BLUE, BOLD_GREEN, tab[tmp->token], BOLD_BLUE, STOP_COLOR);
		else
			ft_printf("%sNODE %d: [%s%s%s]\n | ^\n v |\n%s", BOLD_BLUE, i, BOLD_GREEN, tmp->data->content[0], BOLD_BLUE, STOP_COLOR);
		tmp = tmp->next;
		i++;
	}
	if ((int)tmp->token >= 0)
		ft_printf("%sNODE %d: [%s%s%s]--->[%s%s%s]\n | \n v\n%s", BOLD_BLUE,  i, BOLD_GREEN, tmp->data->content[0], BOLD_BLUE, BOLD_GREEN, tab[tmp->token], BOLD_BLUE, STOP_COLOR);
	else
		ft_printf("%sNODE %d: [%s%s%s]\n | \n v\n%s", BOLD_BLUE,  i, BOLD_GREEN, tmp->data->content[0], BOLD_BLUE, STOP_COLOR);
	ft_printf("%sNULL\n%s", BOLD_BLUE, STOP_COLOR);
}
