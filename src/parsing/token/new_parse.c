/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:33:04 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:13:13 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "lexer.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include "tree.h"
#include "quote.h"
#include <stdlib.h>
#include <errno.h>

static int	find_len_for_new_line(t_token *head)
{
	int		j;
	int		len;

	len = 0;
	while (head)
	{
		j = 0;
		while (head->content[0][j])
			j++;
		len += j + 1;
		if (head->token == EXPAND)
			len += 2;
		head = head->next;
	}
	return (len);
}

static void	copy_expand(t_token *node, char **line, int *pos, int i)
{
	int		len;
	bool	flag;

	len = ft_strlen(&node->content[0][i]);
	flag = false;
	if (node->content[0][i] && node->content[0][i] == ' ')
	{
		(*line)[*pos] = ' ';
		(*pos)++;
		i++;
		flag = true;
	}
	(*line)[*pos] = '"';
	(*pos)++;
	ft_memcpy(&(*line)[*pos], &node->content[0][i], len);
	if (flag)
		*pos += len - 1;
	else
		*pos += len;
	(*line)[*pos] = '"';
	(*pos)++;
	(*line)[*pos] = ' ';
}

static void	case_is_expand(t_token *node, int *pos, char **line)
{
	int		i;
	bool	flag;

	i = 0;
	flag = false;
	if (node->error != LITERAL_EXPAND)
	{
		i = ft_countword(node->content[0]);
		if (i > 1)
		{
			flag = true;
			i = 0;
			while (node->content[0][i] && !ft_isspace(node->content[0][i]))
			{
				(*line)[*pos] = node->content[0][i];
				i++;
				(*pos)++;
			}
		}
	}
	if (!flag)
		i = 0;
	copy_expand(node, line, pos, i);
}

static char	*create_new_line(t_token *head)
{
	int		len;
	char	*line;
	int		pos;

	len = find_len_for_new_line(head);
	line = ft_calloc(sizeof(char), len + 1);
	if (!line)
		return (NULL);
	pos = 0;
	while (head)
	{
		if (head->token == EXPAND || head->token == WILDCARD)
			case_is_expand(head, &pos, &line);
		else
		{
			len = ft_strlen(head->content[0]);
			ft_memcpy(&line[pos], head->content[0], len);
			pos += len;
			line[pos] = ' ';
		}
		head = head->next;
	}
	line[pos] = '\0';
	return (line);
}

bool	parse_again(t_token **head, t_var *list_env, bool *flag)
{
	char	*line;

	line = create_new_line(*head);
	if (!line)
		return (false);
	free_parse(*head);
	*head = NULL;
	if (!lexing_and_tokenize(line, head, list_env, flag)
		|| !error_one_quote(head))
	{
		free(line);
		return (false);
	}
	free(line);
	if (!error_one_parenthesis(head))
	{
		errno = MEM_ALLOC;
		return (false);
	}
	return (true);
}
