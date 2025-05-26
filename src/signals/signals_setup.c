
#include <signals.h>
#include "libft.h"
#include "signals.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

int setup_pipe_signals(struct sigaction *sa_ignore, struct sigaction *sa_orig)
{
	sigemptyset(&sa_ignore->sa_mask);
	sa_ignore->sa_handler = SIG_IGN;
	sa_ignore->sa_flags = 0;
	sigaction(SIGINT, sa_ignore, sa_orig);
	return (0);
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
