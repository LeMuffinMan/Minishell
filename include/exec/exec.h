/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:31:45 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:45:32 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"
# include "tree.h"
# include <sys/wait.h>

// ULTRABOMUS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int		builtin_source(char *arg, t_var **env);

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

int		builtin_cd(char **arg, t_var **env);
int		builtin_echo(char **arg);
int		builtin_env(t_var **env, char **arg);
int		builtin_exit(char **arg, t_tree **ast, t_lists *lists);
int		builtin_pwd(t_var **env);
int		builtin_unset(t_var **env, char **arg);
int		builtin_export(t_var **env, char **arg);

t_tree	*parse(char *line, t_var *new_env, t_lists *lists);

#endif
