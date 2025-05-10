/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:38:26 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>

extern volatile sig_atomic_t g_signal;

// void	setup_child_signals(void);
void	handle_sig(int sig);
void do_nothing_sig(int sig);
int		set_signals(int mode);
void	setup_child_signals(void);
// void handle_child_sigint(int sig);
void handle_child_sigint(void);
void	setup_child_signals(void);
void	setup_parent_signals(void);

#endif
