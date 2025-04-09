/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:12:50 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 17:54:33 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

# include "token.h"

# define E_IO 5
# define MEM_ALLOC 12

enum e_type;
typedef	enum e_type t_type;

typedef struct s_token
{
	int				error;
	struct s_token	*next;
	struct s_token	*prev;
	char			**content;
	enum e_type	token;
}	t_token;

void	add_back(t_token **head, char *str);
void	free_parse(t_token *stack, const char *str, int error);

#endif