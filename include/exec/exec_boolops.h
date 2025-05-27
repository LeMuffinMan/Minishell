/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_boolops.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:59:04 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:45:40 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_BOOLOPS_H
# define EXEC_BOOLOPS_H

# include "structs.h"

int		exec_boolop(t_tree **ast, t_lists *lists);
int		exec_parenthesis_child(t_tree **ast, t_lists *lists);
int		exec_parenthesis(t_tree **ast, t_lists *lists);
int		exec_group_cmd(t_tree **ast, t_lists *lists);
int		exec_group_boolop(t_tree **ast, t_lists *lists);
int		exec_pipe(t_tree **ast, t_lists *lists);
t_tree	*parse(char *line, t_var *list_env, t_lists *lists);
int		exec_ast(t_tree **ast, t_lists *lists);
int		redirect_stdio(t_tree **ast, t_lists *lists);
int		exec_cmd(t_tree **ast, t_lists *lists);
int		close_origin_fds(int origin_fds[2]);
void	free_lists(t_lists *lists);

#endif
