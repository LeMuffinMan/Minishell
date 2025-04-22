/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:48:37 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:49:49 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

int free_pipes(t_pipe **pipes);
int free_pids(t_pids **pids);
int add_pipe(int fd[2], t_pipe **pipes);
int	wait_children(pid_t last_child, pid_t first_child);

#endif

