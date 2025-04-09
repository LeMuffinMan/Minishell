/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:12:50 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 04:00:46 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

# include "token.h"

# define E_IO 5
# define MEM_ALLOC 12

enum e_token;
typedef	enum e_token t_token;

typedef struct s_parse
{
	int				error;
	struct s_parse	*next;
	struct s_parse	*prev;
	char			**content;
	enum e_token	token;
}	t_parse;

void	add_back(t_parse **head, char *str);
void	free_parse(t_parse *stack, const char *str, int error);

#endif