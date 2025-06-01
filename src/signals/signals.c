/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:23:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:05:29 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "signals.h"
#include "structs.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int		g_signal = 0;

//renommer 
void	do_nothing_sig(int sig)
{
	(void)sig;
}

void	handle_sig(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		do_nothing_sig(sig);
	}
}

void	handle_child_sigquit(int sig)
{
	(void)sig;
	exit(131);
}

void	handle_child_sigint(int sig)
{
	(void)sig;
	exit(130);
}

void	collect_signal_exit_code(t_lists *lists)
{
	if (g_signal != 0)
	{
		lists->exit_code = g_signal;
		g_signal = 0;
	}
}
