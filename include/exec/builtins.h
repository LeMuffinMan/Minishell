/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:36 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:21:38 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "structs.h"
# include "tree.h"


//ultrabonus
int builtin_source(char *arg, t_var **env);

// builtins/export
int		free_array(char **array);
int		compare_keys(char *key1, char *key2);
int		builtin_export(t_var **env, char **arg);
t_var	*is_known_key(t_var **env, char *key);

// builtins/cd
int		builtin_cd(char **arg, t_var **env);

// builtins/echo
int		builtin_echo(char **arg);

// builtins/env
int		builtin_env(t_var **env, char **arg);

// builtins/exit
int		builtin_exit(char **arg, t_lists **lists);

// builtins/pwd
int		builtin_pwd(t_var **env);

// builtins/unset
int		builtin_unset(t_var **env, char **arg);
int		env_size(t_var *env);

// builtins_utils
int		sort_var(t_var **env);
int		is_only_numeric_argument(char *s);
int		print_var(t_var **env);
int		array_size(char **array);

// export_utils
int		add_new_var(t_var **env, char **key_value);
char	*trim_operator(char *s);
int		is_increment_operator(char *s);
char	**concat_var(char **arg);
int		is_valid_identifier(char *var);


#endif
