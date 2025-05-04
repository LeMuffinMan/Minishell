/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:23:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:39:02 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "signals.h"
#include <unistd.h>

volatile sig_atomic_t g_signal = 0;

void	handle_sig(int sig)
{
  g_signal = sig; //doc pour volatile et sig_atomic_t
	if (sig == SIGINT)
	{
	  write(STDOUT_FILENO, "\n", 1);
	  rl_on_new_line();
	  rl_replace_line("", 0);
	  rl_redisplay();
  }
  if (sig == SIGQUIT)
  {
	  rl_replace_line("", 0);
	  rl_redisplay();
  }
}

void do_nothing_sig(int sig)
{
  (void)sig;
}

int	set_signals(int mode)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sa.sa_flags = SA_RESTART;
	if (mode == 0)
	  sa.sa_handler = handle_sig;
	else
	  sa.sa_handler = do_nothing_sig;
	if ((sigaction(SIGINT, &sa, NULL)) == -1)
	  return (-1);
	if ((sigaction(SIGQUIT, &sa, NULL)) == -1)
	  return (-1);
	/* sa.sa_handler = SIG_IGN; // On ignore SIGQUIT dans le shell principal */
	return (0);
}

