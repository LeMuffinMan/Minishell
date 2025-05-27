/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:43 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:47:14 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "structs.h"

int		add_back_var(t_var **lst, char *s, int mode);
char	*get_value(t_var **env, char *key);
int		update_env(t_var **env);
int		update_last_arg_var(t_var **env, char **content);
t_var	*is_known_key(t_var **env, char *key);
int		update_exit_code_var(t_var **env, int exit_code);
t_var	*get_key_node(t_var **env, char *key);
t_var	*update_if_known_key(t_var **env, char *var, char *s, int inc);
int		add_or_update_var(t_var **env, char *var);
t_var	*is_known_exported_key(t_var **env, char *key);
int		malloc_free_string(char *s);
int		free_array(char **array);
int		swap_nodes(t_var *n1, t_var *n2);
int		compare_keys(char *key1, char *key2);
void	free_list(t_var **l);

#endif
