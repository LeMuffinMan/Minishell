/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:13:42 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/10 20:51:46 by asinsard         ###   ########lyon.fr   */
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

void	parse_line(char *str, t_lexer **list);
void	free_lexer(t_lexer *head, const char *str, int error);

#endif