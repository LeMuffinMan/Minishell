/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error_handlers.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:00:54 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:10:25 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_ERROR_HANDLERS_H
# define MALLOC_ERROR_HANDLERS_H

# include "structs.h"

int		malloc_free_string(char *s);
void	malloc_error_freelists_exit(t_lists *lists);
int		malloc_error_close_free_pipes(int pipefd[2], t_pipe **pipes);
int		handle_pipe_error(t_lists *lists);
int		malloc_error_parenthesis_child(t_lists *lists, t_tree **ast_to_free);
int		builtin_exit_malloc_error(t_lists *lists, char *s);
int		malloc_error_close_free_exit(t_lists *lists);
void	free_lists(t_lists *lists);
int		free_pipes(t_pipe **pipes);
int		close_origin_fds(int origin_fds[2]);
int		malloc_error_cmd(char *to_free);

#endif
