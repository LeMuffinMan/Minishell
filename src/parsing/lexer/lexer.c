/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:19:05 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 15:49:46 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "display.h"
#include <stdio.h>
#include <errno.h>

void	free_lexer(t_lexer *head)
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
}

static t_lexer	*add_new_node(char *str)
{
	t_lexer	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_lexer));
	if (!new_node)
	{
		errno = MEM_ALLOC;
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->arg = ft_strdup(str);
	if (!new_node->arg)
	{
		errno = MEM_ALLOC;
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
	{
		errno = MEM_ALLOC;
		free_lexer(*head);
		return ;
	}
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

	i = 1;
	while (str[i])
	{
		if (str[i - 1] != '\\' && ((str[i - 1] == '|' && str[i] != '|')
				|| (str[i - 1] == '<' && str[i] != '<')
				|| (str[i - 1] == '>' && str[i] != '>')
				|| ((str[i - 1] == '(')) || (str[i - 1] == ')')))
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
	alloc_operator_to_lexer(str, i, list);
	if (ft_strlen(str) < (size_t)i)
		i--;
	return (i);
}

bool	parse_line(char *str, t_lexer **list)
{
	int		i;

	i = 0;
	if (!str)
		return (false);
	errno = 0;
	while (str[i] && errno != MEM_ALLOC)
	{
		if (ft_isspace(str[i]))
			i += alloc_space_to_lexer(&str[i], list);
		else if (str[i] == '|' || str[i] == '&'
			|| str[i] == '<' || str[i] == '>'
			|| str[i] == '(' || str[i] == ')')
			i += parse_operator(&str[i], str[i], list);
		else if (str[i] == '"' || str[i] == '\'')
			i += alloc_quote_to_lexer(&str[i], str[i], list);
		else
			alloc_word_to_lexer(str, &i, list);
	}
	if (errno == MEM_ALLOC)
		return (false);
	return (true);
}
