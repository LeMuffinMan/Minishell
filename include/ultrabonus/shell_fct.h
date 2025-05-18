/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_fct.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:34:04 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 17:34:05 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_FCT_H
# define SHELL_FCT_H

#include "structs.h"

t_shell_fct *is_a_known_shell_fct(char *word, t_shell_fct **shell_functions);
char **expand_functions_args(char **args, char **shell_fct_content);
char **gather_function_args(t_tree **ast, char **content);
int exec_shell_fct(t_tree **ast, t_lists *lists, t_shell_fct *shell_fct);

#endif
