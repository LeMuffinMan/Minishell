/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 02:33:29 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 18:28:34 by oelleaum         ###   ########.fr       */
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

# ifndef DEBUG
#  define DEBUG 0
# endif

# include "lexer.h"
# include "list.h"
# include "tree.h"

struct					s_token;
typedef struct s_token	t_token;

struct					s_lexer;
typedef struct s_lexer	t_lexer;

void					display_list(t_token *head, int debug);
void					display_lexer(t_lexer *head, int debug);
void					display_ast(t_tree *root, int debug);
int						error_cmd_is_a_directory(char *cmd);
int						print_export_line(t_var *var);

#endif
