/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 01:45:03 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/30 17:07:02 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>

int	ft_is_operator(int c)
{
	if (c == '|' || c == '&'
		|| c == '<' || c == '>'
		|| c == '(' || c == ')')
		return (1);
	return (0);
}

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
		&& str[*i] != '(' && str[*i] != ')'
		&& str[*i] != '"' && str[*i] != '\'')
		(*i)++;
	word = ft_strndup(&str[start], *i - start);
	if (!word)
		free_lexer(*list, "Malloc failed in parse_line", MEM_ALLOC);
	add_lexer_back(list, word);
	free(word);
}

int	alloc_space_to_lexer(char *str, t_lexer **list)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	word = ft_strndup(str, i);
	if (!word)
		free_lexer(*list, "Malloc failed in 'alloc_space_to_lexer'", MEM_ALLOC);
	add_lexer_back(list, word);
	free(word);
	return (i);
}

int	alloc_quote_to_lexer(char *str, char c, t_lexer **list)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == c)
		{
			if (str[i - 1] != '\\')
				break ;
		}
	}
	if (str[i] && str[i] == c)
		i++;
	word = ft_strndup(str, i);
	if (!word)
		free_lexer(*list, "Malloc failed in 'alloc_space_to_lexer'", MEM_ALLOC);
	add_lexer_back(list, word);
	free(word);
	return (i);
}
