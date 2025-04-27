/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:19:05 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/24 04:29:18 by asinsard         ###   ########lyon.fr   */
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

void	add_lexer_back(t_lexer **head, char *str)
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

static int	parse_operator(char *str, char c, t_lexer **list)
{
	int		i;
	char	*res;
	int		len;

	i = 1;
	len = ft_strlen(str);
	while (str[i])
	{
		if (c != '"' && c != '\'' && ((str[i - 1] == '|' && str[i] != '|') 
		|| (str[i - 1] == '<' && str[i] != '<')))
		{
			i -= 1;
			break ;
		}
		else if (str[i] == c && str[i - 1] == '\\')
			i++;
		else if (str[i] == c)
			break ;
		i++;
	}
	i++;
	if (i >= len)
		i--;
	alloc_operator_to_lexer(str, &res, i, list);
	return (i);
}

void	parse_line(char *str, t_lexer **list)
{
	int		i;
	
	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == ' ')
			i++;
		else if (str[i] == '|' || str[i] == '&'
			|| str[i] == '"' || str[i] == '\''
			|| str[i] == '<' || str[i] == '>')
			i += parse_operator(&str[i], str[i], list);
		else
			alloc_word_to_lexer(str, &i, list);
	}
	check_parenthesis(list);
}
