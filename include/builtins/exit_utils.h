/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:04:41 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:18:54 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_UTILS_H
# define EXIT_UTILS_H

#include "structs.h"

int	is_only_numeric_argument(char *s);
int		close_origin_fds(int origin_fds[2]);
void	free_lists(t_lists *lists);

#endif
