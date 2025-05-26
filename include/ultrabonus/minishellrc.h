/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellrc.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:22:01 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:59:23 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELLRC_H
# define MINISHELLRC_H

# include "structs.h"

char	*find_minishellrc(t_var **env, char *path);
int		load_minishellrc(t_var **env, t_alias **aliases,
			t_shell_fct **shell_fcts, char *path);

#endif
