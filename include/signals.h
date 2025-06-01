/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:09:43 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

extern int	g_signal;

void		handle_sig(int sig);
void		do_nothing_sig(int sig);
int			set_signals(int mode);
void		setup_child_signals(void);
void		handle_child_sigint(int sig);
void		setup_child_signals(void);
void		setup_parent_signals(void);
int			setup_pipe_signals(struct sigaction *sa_ignore,
				struct sigaction *sa_orig);
int			wait_children(pid_t last_child, pid_t first_child);
void		handle_child_sigquit(int sig);
void		setup_here_doc_signals(void);
void		sigint_here_doc_handler(int sig);

#endif
