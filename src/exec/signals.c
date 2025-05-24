/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:23:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/13 14:12:48 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "signals.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// pour CTRL bckslsh : child : on veut core dump + imprimer le message
////et ne rien faire dans un parent

//volatile ou int ?
int	g_signal = 0;

void	do_nothing_sig(int sig)
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

void	setup_child_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = handle_child_sigquit;
	sigaction(SIGQUIT, &sa, NULL);
}
void	sigint_prompt_handler(int sig)
{
  (void)sig;
  g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_prompt_handler);
	signal(SIGQUIT, SIG_IGN);
}


