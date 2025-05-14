/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:33:59 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 17:34:00 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIASES_H
# define ALIASES_H 

#include "structs.h"

t_alias *is_a_known_alias(char *word, t_alias **aliases);
char *expand_alias(char **content, t_alias **alias);
int exec_alias(t_tree **ast, t_lists **lists, t_alias *alias);

#endif
