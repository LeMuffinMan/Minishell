/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:33:04 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 00:35:24 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "lexer.h"
#include "token.h"
#include "list.h"
#include "libft.h"
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
		head = head->next;
	}
	return (len);
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
		len = ft_strlen(head->content[0]);
		ft_memcpy(&line[pos], head->content[0], len);
		pos += len;
		line[pos] = ' ';
		head = head->next;
	}
	line[pos] = '\0';
	return (line);
}

bool	parse_again(t_token **head, t_var *list_env, bool *flag)
{
	char	*line;
	t_lexer	*lexer;

	line = create_new_line(*head);
	if (!line)
		return (false);
	lexer = NULL;
	free_parse(*head, NULL, MEM_ALLOC);
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
	handle_wildcard(head, *flag);
	return (true);
}
