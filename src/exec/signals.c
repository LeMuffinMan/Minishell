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

#include <stdio.h>
#include <readline/readline.h>
#include "signals.h"
#include <unistd.h>
#include <stdlib.h>


//pour CTRL bckslsh : child : on veut core dump + imprimer le message 
////et ne rien faire dans un parent 

volatile sig_atomic_t g_signal = 0;

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
	else if (mode == 1)
	  sa.sa_handler = SIG_IGN; // ou do nothing ?
	if ((sigaction(SIGINT, &sa, NULL)) == -1)
	  return (-1);
	if ((sigaction(SIGQUIT, &sa, NULL)) == -1)
	  return (-1);
	/* sa.sa_handler = SIG_IGN; // On ignore SIGQUIT dans le shell principal */
	return (0);
}


void	handle_sig(int sig)
{
  g_signal = sig; //doc pour volatile et sig_atomic_t
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

void 	handle_child_sigquit()
{
	exit (131);
}

void handle_child_sigint()
{
    /* write(STDOUT_FILENO, "\n", 1);  // Force un saut de ligne */
    exit(130);  // Code de sortie standard pour SIGINT (128 + 2)
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_child_sigint; 
	// SIGINT par défaut dans les enfants
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	// SIGQUIT par défaut dans les enfants
	sa.sa_handler = handle_child_sigquit;
	sigaction(SIGQUIT, &sa, NULL);
}

void sigint_prompt_handler()
{
	  write(STDOUT_FILENO, "\n", 1);
	  rl_on_new_line();
	  rl_replace_line("", 0);
	  rl_redisplay();
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_prompt_handler);   // Ctrl+C géré dans le prompt
	signal(SIGQUIT, SIG_IGN);         // Ctrl+\ ignoré dans le shell
}
