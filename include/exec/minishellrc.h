/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellrc.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:22:01 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:22:02 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELLRC_H
# define MINISHELLRC_H

#include "structs.h"

int	find_minishellrc(t_var **env, char *path);
int	load_minishellrc(t_var **env, char *path);

#endif
