/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error_handlers.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:00:54 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:19:45 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_ERRORS_HANDLERS_H
# define MALLOC_ERRORS_HANDLERS_H

# include "structs.h"

int		malloc_free_string(char *s);
void	malloc_error_freelists_exit(t_lists *lists);
int		malloc_error_close_free_pipes(int pipefd[2], t_pipe **pipes);
int		handle_pipe_error(t_lists *lists);
int		malloc_error_parenthesis_child(t_lists *lists, t_tree **ast_to_free);
int		builtin_exit_malloc_error(t_lists *lists, char *s);

#endif
