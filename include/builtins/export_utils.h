/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:58:29 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:58:37 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_UTILS_H
# define EXPORT_UTILS_H

# include "structs.h"

char	*get_full_variable_declaration(char **arg, int i, char **s);
int		is_var_exportation(char *s);
int		is_var_declaration(char *arg);
int		export_without_argument(t_var **env);
int		add_new_var(t_var **env, char **key_value);
char	*trim_operator(char *s);
int		is_increment_operator(char *s);
char	**concat_var(char **arg);
int		is_valid_identifier(char *var);

#endif
