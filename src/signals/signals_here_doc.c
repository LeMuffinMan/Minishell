/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_here_doc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:48:49 by oelleaum          #+#    #+#             */
/*   Updated: 2025/06/01 14:56:45 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signals.h>
#include <unistd.h>

void	sigint_here_doc_handler(int sig)
{
	(void)sig;
	close(0);
	g_signal = 130;
}

void	setup_here_doc_signals(void)
{
	signal(SIGINT, sigint_here_doc_handler);
}
