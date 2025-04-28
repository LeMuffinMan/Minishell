/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 01:45:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/28 13:44:40 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>

void	alloc_operator_to_lexer(char *str, int i, t_lexer **list)
{
	char	*res;

	res = ft_strndup(str, i);
	if (!res)
		free_lexer(*list,
			"Malloc failed in function 'alloc_lexer'", MEM_ALLOC);
	add_lexer_back(list, res);
	free(res);
}

void	alloc_word_to_lexer(char *str, int *i, t_lexer **list)
{
	int		start;
	char	*word;

	start = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '&'
		&& str[*i] != '|' && str[*i] != '<' && str[*i] != '>'
		&& str[*i] != '(' && str[*i] != ')')
		(*i)++;
	word = ft_strndup(&str[start], *i - start);
	if (!word)
		free_lexer(*list, "Malloc failed in parse_line", MEM_ALLOC);
	add_lexer_back(list, word);
	free(word);
}
