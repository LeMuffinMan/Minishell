/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 14:19:04 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "token.h"
#include "list.h"
#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include "signals.h"
#include "utils.h"
#include <sys/stat.h>
#include <errno.h>

bool	verif_name(char *name)
{
	struct stat	status;

	if (stat(name, &status) == 0)
	{
		free(name);
		return (false);
	}
	return (true);
}

bool	extract_stdin(int fd, char *limiter)
{
	int		len;
	char	*line;
	char	*tmp;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf(
				"minishell: here-doc delimited by end-of-file (wanted `%s')\n",
				limiter);
			break ;
		}
		tmp = line;
		line = ft_strjoin(line, "\n");
		free(tmp);
		len = ft_strlen(limiter);
		if (ft_strncmp(line, limiter, len) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (false);
		}
		free(line);
	}
	return (true);
}

/* void	here_doc_signal_handler(int signum) */
/* { */
/* 	if (signum == SIGINT) */
/* 		g_signal = SIGINT; */
/* } */

/* void	setup_here_doc_signals(void) */
/* { */
/* 	struct sigaction	sa; */
/**/
/* 	sa.sa_handler = here_doc_signal_handler; */
/* 	sigemptyset(&sa.sa_mask); */
/* 	sa.sa_flags = 0; */
/* 	sigaction(SIGINT, &sa, NULL); */
/* 	 */
/* 	// Ignorer SIGQUIT */
/* 	sa.sa_handler = SIG_IGN; */
/* 	sigaction(SIGQUIT, &sa, NULL); */
/* } */

//j'ai regulierement  minishell: Problem with here_doc creation en essayant de faire plusieurs here_docs
bool	create_here_doc(t_token *node, t_lists *lists, bool *sig_hd)
{
	int		fd;
	char	*limiter;
	int status;
	pid_t pid;
	struct sigaction sa_orig_int, sa_orig_quit;
	struct sigaction sa;
    
  sigaction(SIGINT, NULL, &sa_orig_int);
  sigaction(SIGQUIT, NULL, &sa_orig_quit);
	fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (false);
	limiter = ft_strdup(node->content[1]);
	if (!limiter)
	{
		close(fd);
		return (false);
	}
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
	g_signal = 0;
	pid = fork();
	if (pid < 0)
	{
    close(fd);
    free(limiter);
    sigaction(SIGINT, &sa_orig_int, NULL);
    sigaction(SIGQUIT, &sa_orig_quit, NULL);
    return (false); // si le pid foire on veut faire remonter l'erreur 
	}
	if (pid == 0)
	{
    signal(SIGINT, SIG_DFL); //ferme les hd un par un pour le moment
    signal(SIGQUIT, SIG_IGN);
		close_origin_fds(lists->origin_fds);
		free_lists(lists);
		if (!extract_stdin(fd, limiter))
			exit(EXIT_FAILURE);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        lists->exit_code = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
      if (WTERMSIG(status) == SIGINT)
      {
      	*sig_hd = 0;
        lists->exit_code = 130; // pour le moment on n'arrive pas a recuperer le bon exit code 
				write(1, "\n", 1);
      }
      else
          lists->exit_code = 1;  // a changer !!
    }
    sigaction(SIGINT, &sa_orig_int, NULL);
		sigaction(SIGQUIT, &sa_orig_quit, NULL);
	}
	close(fd);
	free(limiter);
	return (true);
}

bool	handle_here_doc(t_token **head, t_lists *lists)
{
	t_token	*tmp;
	bool sig_hd; //ce booleen permet d'empecher d'ouvrir d'autres hd si un des hd a recu un SIGINT
	//mais ca provoque une autre erreur

	sig_hd = 1;
	if (!head || !*head)
		return (false);
	if (!create_hd_name(head))
	{
		free_parse(*head, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return (false);
	}
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == HD && sig_hd)
		{
			if (!create_here_doc(tmp, lists, &sig_hd))
			{
				free_parse(*head, NULL, MEM_ALLOC);
				errno = MEM_ALLOC;
				return (false);
			}
		}
		tmp = tmp->next;
	}
	return (true);
}
