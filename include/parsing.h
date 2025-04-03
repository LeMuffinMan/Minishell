/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:26:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 01:39:47 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define BOLD_GREEN "\e[1;32m"
# define BOLD_RED "\e[1;31m"
# define BOLD_BLUE "\e[1;34m"
# define BOLD_YELLOW "\e[1;33m"
# define BOLD_PURPLE "\e[1;35m"
# define BOLD_CYAN "\e[1;36m"
# define STOP_COLOR "\e[0m"

# define MEM_ALLOC 12


# include <stdbool.h>

typedef struct s_data
{
	char	**av;
	char	**envp;
	int		ac;
	char	**content;
}	t_data;

typedef enum e_token
{
	APPEND,
	CMD,
	D_QUOTE,
	HD,
	LIM,
	O_AND,
	O_OR,
	PIPE,
	R_IN,
	S_QUOTE,
	TRUNC,
	WILDCARD
}	t_token;

typedef struct s_parse
{
	struct s_parse	*next;
	struct s_parse	*prev;
	t_data			*data;
	t_token			token;
}	t_parse;

/* ----------------------STACK---------------------- */
/* -------------------Create Stack------------------ */
void	add_back(t_parse **head, char *str);
t_parse	*add_new_node(char *str);

/* -------------------Stack Utils------------------- */
void	display_list(t_parse *head);
void	free_parse(t_parse *stack);

/* ---------------------PARSING--------------------- */
/* ----------------------Token---------------------- */
void	assign_token(t_parse **head);

#endif
