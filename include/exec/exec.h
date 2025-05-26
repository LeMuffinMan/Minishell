/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:31:45 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:58:59 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"
# include "tree.h"
# include <sys/wait.h>

// exec.c
int		exec_ast(t_tree **ast, t_lists *lists);
int		exec_cmd(t_tree **ast, t_lists *lists);
int		builtins(char **arg, t_lists *lists);

// errors.c
int		error_cmd(char *cmd, int error_code);
int		print_error_file_opening(char *file, char *error, int error_code);
int		error_not_valid_identifier(char *s);
int		print_error_is_a_directory(char *file);
int		print_perm_error(char *file);

t_tree	*parse(char *line, t_var *new_env, t_lists *lists);

#endif
