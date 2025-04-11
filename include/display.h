/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 02:33:29 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/11 00:27:20 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

# define BOLD_GREEN "\e[1;32m"
# define BOLD_RED "\e[1;31m"
# define BOLD_BLUE "\e[1;34m"
# define BOLD_YELLOW "\e[1;33m"
# define BOLD_PURPLE "\e[1;35m"
# define BOLD_CYAN "\e[1;36m"
# define STOP_COLOR "\e[0m"

# include "list.h"
# include "lexer.h"
# include "tree.h"

struct s_token;
typedef struct s_token t_token;

struct s_lexer;
typedef struct s_lexer t_lexer;

void	display_list(t_token *head);
void	display_lexer(t_lexer *head);
void	print_ast(t_tree *node, int depth);

#endif