/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdio.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:46:09 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:46:10 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_STDIO_H
# define REDIRECT_STDIO_H

# include "token.h"
# include "tree.h"

int	file_check(char *file, t_type type, int error_code);
int	is_a_directory(char *name);
int	open_dup2_close(char *file, t_type type);
int	error_cmd(char *cmd, int error_code);
int	redirect_stdio(t_tree **ast, t_lists *lists);
int	exec_cmd(t_tree **ast, t_lists *lists);

#endif
