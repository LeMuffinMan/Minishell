/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_boolops.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:59:04 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:59:10 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_BOOLOPS_H
# define EXEC_BOOLOPS_H

# include "structs.h"

int	exec_boolop(t_tree **ast, t_lists *lists);
int	exec_parenthesis_child(t_tree **ast, t_lists *lists);
int	exec_parenthesis(t_tree **ast, t_lists *lists);
int	exec_group_cmd(t_tree **ast, t_lists *lists);
int	exec_group_boolop(t_tree **ast, t_lists *lists);

#endif
