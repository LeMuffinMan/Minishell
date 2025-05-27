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

pid_t manage_here_doc_fork(int fd, t_lists *lists, char *limiter, t_token *node)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
    close(fd);
    free(limiter);
		return (-1);
	}
	if (pid == 0)
	{
		close_origin_fds(lists->origin_fds);
		free_lists(lists);
		free_parse(node, NULL, 0);
		if (!extract_stdin(fd, limiter))
		{
			free(limiter);
			close(fd);
			exit(EXIT_FAILURE);
		}
		free(limiter);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	return (pid);
}

bool free_lim_close_fd(char *limiter, int fd)
{
	free(limiter);
	if (close(fd) == -1)
		return (false);
	return (true);
}

bool wait_here_doc(pid_t pid, t_lists *lists, bool *sig_hd)
{
	int status;

	waitpid(pid, &status, 0);
  if (WIFEXITED(status))
      lists->exit_code = WEXITSTATUS(status);
  else if (WIFSIGNALED(status))
  {
    if (WTERMSIG(status) == SIGINT)
    {
      *sig_hd = 0;
      lists->exit_code = 130; // pour le moment on n'arrive pas a recuperer le bon exit code 
			write(1, "\n", 1);
			return (false);
    }
    else
        lists->exit_code = 1;  // a changer !!
  }
  return (true);
}

char *get_limiter(char *s, int fd)
{
	char *limiter;

	limiter = ft_strdup(s);
	if (!limiter)
	{
		close(fd);
		return (NULL);
	}
	return (limiter);
}

bool create_here_doc(t_token *node, t_lists *lists, bool *sig_hd)
{
	int fd;
	char *limiter;
	pid_t pid;

	fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (false);
	limiter = get_limiter(node->content[1], fd);
	if (!limiter)
		return (false);
	pid = manage_here_doc_fork(fd, lists, limiter, node);
	if (pid < 0)
		return (false);
	else
	{
		if (!free_lim_close_fd(limiter, fd))
			return (false);
		if (!wait_here_doc(pid, lists, sig_hd))
			return (false);
  }
	return (true);
}

/* bool	create_here_doc(t_token *node, t_lists *lists, bool *sig_hd) */
/* { */
/* 	int		fd; */
/* 	char	*limiter; */
/* 	int status; */
/* 	pid_t pid; */
/* 	struct sigaction sa_orig_int, sa_orig_quit; */
/* 	struct sigaction sa; */
/*      */
/*   sigaction(SIGINT, NULL, &sa_orig_int); */
/*   sigaction(SIGQUIT, NULL, &sa_orig_quit); */
/* 	fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); */
/* 	if (fd == -1) */
/* 		return (false); */
/* 	limiter = ft_strdup(node->content[1]); */
/* 	if (!limiter) */
/* 	{ */
/* 		close(fd); */
/* 		return (false); */
/* 	} */
/*   sa.sa_handler = SIG_IGN; */
/*   sigemptyset(&sa.sa_mask); */
/*   sa.sa_flags = 0; */
/*   sigaction(SIGINT, &sa, NULL); */
/* 	g_signal = 0; */
/* 	pid = fork(); */
/* 	if (pid < 0) */
/* 	{ */
/*     close(fd); */
/*     free(limiter); */
/*     sigaction(SIGINT, &sa_orig_int, NULL); */
/*     sigaction(SIGQUIT, &sa_orig_quit, NULL); */
/*     return (false); // si le pid foire on veut faire remonter l'erreur  */
/* 	} */
/* 	if (pid == 0) */
/* 	{ */
/*     signal(SIGINT, SIG_IGN); //ferme les hd un par un pour le moment */
/*     signal(SIGQUIT, SIG_IGN); */
/* 		close_origin_fds(lists->origin_fds); */
/* 		free_lists(lists); */
/* 		free_parse(node, NULL, 0); */
/* 		if (!extract_stdin(fd, limiter)) */
/* 		{ */
/* 			free(limiter); */
/* 			close(fd); */
/* 			exit(EXIT_FAILURE); */
/* 		} */
/* 		free(limiter); */
/* 		close(fd); */
/* 		exit(EXIT_SUCCESS); */
/* 	} */
/* 	else */
/* 	{ */
/* 		free(limiter); */
/* 		close(fd); */
/*     waitpid(pid, &status, 0); */
/*     if (WIFEXITED(status)) */
/*     { */
/*         lists->exit_code = WEXITSTATUS(status); */
/*     } */
/*     else if (WIFSIGNALED(status)) */
/*     { */
/*       if (WTERMSIG(status) == SIGINT) */
/*       { */
/*       	*sig_hd = 0; */
/*         lists->exit_code = 130; // pour le moment on n'arrive pas a recuperer le bon exit code  */
/* 				write(1, "\n", 1); */
/* 				return (false); */
/*       } */
/*       else */
/*           lists->exit_code = 1;  // a changer !! */
/*     } */
/*     sigaction(SIGINT, &sa_orig_int, NULL); */
/* 		sigaction(SIGQUIT, &sa_orig_quit, NULL); */
/* 	} */
/* 	return (true); */
/* } */

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
				if (lists->exit_code == 130)
					errno = 130;
				else 
					errno = MEM_ALLOC;
				return (false);
			}
		}
		tmp = tmp->next;
	}
	return (true);
}
