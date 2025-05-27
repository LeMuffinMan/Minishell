/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:23:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:41:21 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "signals.h"
#include "structs.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// HD : CTRL \ PAS BON !!!

int		g_signal = 0;

void	do_nothing_sig(int sig)
{
	// renommer ou utiliser SIGIGN
	(void)sig;
}

void	handle_sig(int sig)
{
	g_signal = sig;
	if (sig == SIGINT) // CTRL + C
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT) // CTRL + backslash
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
	/* write(STDOUT_FILENO, "\n", 1);  // Force un saut de ligne */
	exit(130); // Code de sortie standard pour SIGINT (128 + 2)
}

void	collect_signal_exit_code(t_lists *lists)
{
	if (g_signal != 0)
	{
		lists->exit_code = g_signal;
		g_signal = 0; // Réinitialiser le flag de signal
	}
}
