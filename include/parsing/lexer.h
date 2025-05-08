/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:13:42 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/08 19:35:25 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# define MEM_ALLOC 12

typedef struct s_lexer
{
	struct s_lexer	*next;
	struct s_lexer	*prev;
	char			*arg;
}	t_lexer;

void					parse_line(char *str, t_lexer **list);
void					free_lexer(t_lexer *head, const char *str, int error);
void					add_lexer_back(t_lexer **head, char *str);
void					alloc_operator_to_lexer(char *str, int i, t_lexer **list);
void					alloc_word_to_lexer(char *str, int *i, t_lexer **list);
int						alloc_space_to_lexer(char *str, t_lexer **list);
int						alloc_quote_to_lexer(char *str, char c, t_lexer **list);


#endif