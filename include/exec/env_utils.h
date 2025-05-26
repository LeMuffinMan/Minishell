/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:43 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:58:48 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "structs.h"

int		init_env(t_var **new_env, char **env, char *name_of_program);
int		add_back_var(t_var **lst, char *s, int mode);
int		add_first_node(t_var **lst, t_var **new, char *s, int mode);
char	*get_value(t_var **env, char *key);
int		update_env(t_var **env);
int		update_last_arg_var(t_var **env, char **content);
t_var	*is_known_key(t_var **env, char *key);
int		print_all_variables(t_var **env);
int		update_exit_code_var(t_var **env, int exit_code);
t_var	*get_key_node(t_var **env, char *key);
int		append_to_var(t_var **node, char **key_value);
int		update_var(t_var **node, char **key_value, int inc);
int		update_existing_value(t_var *node, char *var, int inc);
t_var	*update_if_known_key(t_var **env, char *var, char *s, int inc);
int		add_or_update_var(t_var **env, char *var);
t_var	*is_known_exported_key(t_var **env, char *key);

#endif
