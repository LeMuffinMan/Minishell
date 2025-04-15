/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:19:05 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/14 19:38:13 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "display.h"
#include <stdio.h>

void	free_lexer(t_lexer *head, const char *str, int error)
{
	t_lexer	*tmp;
	t_lexer	*next_node;

	if (!head)
		return ;
	if (head->prev)
	{
		while (head->prev)
			head = head->prev;
	}
	tmp = head;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->arg)
			free(tmp->arg);
		free(tmp);
		tmp = next_node;
	}
	if (str)
		ft_error(str, error);
}

static t_lexer	*add_new_node(char *str)
{
	t_lexer	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->arg = ft_strdup(str);
	if (!new_node->arg)
	{
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

static void	add_lexer_back(t_lexer **head, char *str)
{
	t_lexer	*new_node;
	t_lexer	*tmp;

	new_node = add_new_node(str);
	if (!new_node)
		free_lexer(*head,
			"Malloc failed in function 'add_lexer_back'", MEM_ALLOC);
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
	{
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	tmp->next = new_node;
	new_node->prev = tmp;
}

static int	alloc_lexer(char *str, char c, t_lexer **list)
{
	int		i;
	char	*res;

	i = 1;
	while (str[i])
	{
		if (str[i] == c && str[i - 1] == '\\')
			i++;
		else if (str[i] == c)
			break ;
		i++;
	}
	i++;
	res = ft_strndup(str, i);
	if (!res)
		free_lexer(*list,
			"Malloc failed in function 'alloc_lexer'", MEM_ALLOC);
	add_lexer_back(list, res);
	free(res);
	return (i);
}

void	parse_line(char *str, t_lexer **list)
{
	int		i;
	int		start;
	char	*word;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			i++;
		else if (str[i] == '"' || str[i] == '\'')
			i += alloc_lexer(&str[i], str[i], list);
		else
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'')
				i++;
			word = ft_strndup(&str[start], i - start);
			if (!word)
				free_lexer(*list, "Malloc failed in parse_line", MEM_ALLOC);
			add_lexer_back(list, word);
			free(word);
		}
	}
}
