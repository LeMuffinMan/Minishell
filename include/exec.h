/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:31:45 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:02:04 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "tree.h"
#include <sys/wait.h>
#include "structs.h"


int exec_ast (t_tree **ast, t_var **env);
void	add_back_var(t_var **lst, char *s, int mode);
void	add_first_node(t_var **lst, t_var *new, char *s, int mode);
void	free_list(t_var **l);

//BUILTINS
//builtins/export
int free_array(char **array);
int	compare_keys(char *key1, char *key2);
int builtin_export(t_var **env, char **arg);
//builtins/cd
int	builtin_cd(char **arg, t_var **env);
//builtins/echo
int	builtin_echo(char **arg);
//builtins/env
int builtin_env(t_var **env);
//builtins/exit
int	builtin_exit(char **arg, t_var **env, t_tree **ast);
//builtins/pwd
int	builtin_pwd(void);
//builtins/unset
int builtin_unset(t_var **env, char **arg);
int env_size(t_var *env);

//init
int	init_env(t_var **new_env, char **env, char **arg);

//UTILS
//utils/builtins_utils
int sort_var(t_var **env);
int is_only_numeric_argument(char *s);
int print_var(t_var **env);
int	array_size(char **array);
t_var	*copy_list(t_var **env);
//utils/env_utils
char	*get_value(t_var **env, char *key);
int	update_env(t_var **env);
//utils/prints.c
int	print_sorted_env(t_var **env);
int	print_array(char **array);

//utils.c 
char **lst_to_array(t_var **env);
t_tree	*parse(char *line, char **envp);
int free_pipes(t_pipe **pipes);

//get_prompt
// char *get_prompt (t_var **env);
// int free_prompt(t_prompt *prompt);

#endif
