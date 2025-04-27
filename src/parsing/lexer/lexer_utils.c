/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 01:45:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/24 04:29:32 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>

static void	alloc_content(t_lexer *node, t_lexer **new_node,
							char **new_content, char **old_content)
{
	if (node->arg[0] == '(')
		*new_content = ft_strndup(node->arg, 1);
	else
		*new_content = ft_strndup(&node->arg[ft_strlen(node->arg) - 1], 1);
	if (!*new_content)
		free_lexer(node, "Malloc failed in alloc_content", MEM_ALLOC);
	if (node->arg[0] == '(')
		*old_content = ft_strdup(&node->arg[1]);
	else
		*old_content = ft_strndup(node->arg, ft_strlen(node->arg) - 1);
	if (!*old_content)
	{
		free(*new_content);
		free_lexer(node, "Malloc failed in alloc_content", MEM_ALLOC);
	}
	*new_node = malloc(sizeof(t_lexer));
	if (!*new_node)
	{
		free(*new_content);
		free(*old_content);
		free_lexer(node, "Malloc failed in alloc_content", MEM_ALLOC);
	}
}

static void	change_node_lexer(t_lexer **node)
{
	t_lexer	*new_node;
	char	*new_content;
	char	*old_content;

	alloc_content(*node, &new_node, &new_content, &old_content);
	new_node->arg = new_content;
	free((*node)->arg);
	(*node)->arg = old_content;
	if (new_content[0] == '(')
	{
		new_node->next = *node;
		new_node->prev = (*node)->prev;
		if (new_node->prev)
			new_node->prev->next = new_node;
		(*node)->prev = new_node;
	}
	else
	{
		new_node->next = (*node)->next;
		if (new_node->next)
			new_node->next->prev = new_node;
		new_node->prev = (*node);
		(*node)->next = new_node;
	}
}

void	check_parenthesis(t_lexer **list)
{
	t_lexer	*tmp;

	if (!*list)
		return ;
	tmp = *list;
	while (tmp)
	{
		if (tmp->arg[0] == '(' || tmp->arg[ft_strlen(tmp->arg) - 1] == ')')
		{
			change_node_lexer(&tmp);
			if (tmp->prev->arg[0] == '(' && !tmp->prev->prev)
				*list = tmp->prev;
			tmp = tmp->next;
			if (tmp->arg[ft_strlen(tmp->arg) - 1] == ')')
				tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}

void	alloc_operator_to_lexer(char *str, char **res, int i, t_lexer **list)
{
	*res = ft_strndup(str, i);
	if (!*res)
		free_lexer(*list,
			"Malloc failed in function 'alloc_lexer'", MEM_ALLOC);
	add_lexer_back(list, *res);
	free(*res);
}

void	alloc_word_to_lexer(char *str, int *i, t_lexer **list)
{
	int		start;
	char	*word;
 
	start = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '&'
		&& str[*i] != '|' && str[*i] != '<' && str[*i] != '>')
		(*i)++;
	word = ft_strndup(&str[start], *i - start);
	if (!word)
		free_lexer(*list, "Malloc failed in parse_line", MEM_ALLOC);
	add_lexer_back(list, word);
	free(word);
}
