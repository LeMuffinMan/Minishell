
#include <stdlib.h>
#include <signals.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "token.h"
#include "libft.h"
#include "here_doc.h"

/* static void setup_signal_handlers(struct sigaction *sa_orig_int,  */
/*                                  struct sigaction *sa_orig_quit) */
/* { */
/*     sigaction(SIGINT, NULL, sa_orig_int); */
/*     sigaction(SIGQUIT, NULL, sa_orig_quit); */
/* } */
/**/
/* static int prepare_heredoc(t_token *node, char **limiter) */
/* { */
/*     int fd; */
/**/
/*     fd = open(node->content[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); */
/*     if (fd == -1) */
/*         return (-1); */
/*      */
/*     *limiter = ft_strdup(node->content[1]); */
/*     if (!*limiter) */
/*     { */
/*         close(fd); */
/*         return (-1); */
/*     } */
/*     return fd; */
/* } */
/**/
/* static void setup_here_doc_parent_signals(void) */
/* { */
/*     struct sigaction sa; */
/*      */
/*     sa.sa_handler = SIG_IGN; */
/*     sigemptyset(&sa.sa_mask); */
/*     sa.sa_flags = 0; */
/*     sigaction(SIGINT, &sa, NULL); */
/*     g_signal = 0; */
/* } */
/**/
/* static void handle_child_process(int fd, char *limiter, t_lists *lists) */
/* { */
/*   signal(SIGINT, SIG_DFL); */
/*   signal(SIGQUIT, SIG_IGN); */
/*   close_origin_fds(lists->origin_fds); */
/*   free_lists(lists); */
/*   if (!extract_stdin(fd, limiter)) */
/*       exit(EXIT_FAILURE); */
/*   close(fd); */
/*   exit(EXIT_SUCCESS); */
/* } */
/**/
/* static bool handle_parent_process(pid_t pid, t_lists *lists,  */
/*                                 struct sigaction *sa_orig_int, */
/*                                 struct sigaction *sa_orig_quit) */
/* { */
/*     int status; */
/*      */
/*     waitpid(pid, &status, 0); */
/*     if (WIFEXITED(status)) */
/*     { */
/*         lists->exit_code = WEXITSTATUS(status); */
/*     } */
/*     else if (WIFSIGNALED(status)) */
/*     { */
/*         if (WTERMSIG(status) == SIGINT) */
/*         { */
/*             lists->exit_code = 130; */
/*             write(1, "\n", 1); */
/*             return (false); */
/*         } */
/*         lists->exit_code = 1; */
/*     } */
/*     sigaction(SIGINT, sa_orig_int, NULL); */
/*     sigaction(SIGQUIT, sa_orig_quit, NULL); */
/*     return (true); */
/* } */
/**/
/* bool exec_here_doc_fork_error(int fd, char *limiter, struct sigaction *sa_orig_quit, struct sigaction *sa_orig_int) */
/* { */
/*   close(fd); */
/*   free(limiter); */
/*   sigaction(SIGINT, sa_orig_int, NULL); */
/*   sigaction(SIGQUIT, sa_orig_quit, NULL); */
/*   return (false); */
/* } */
/**/
/**/
/* bool exec_here_doc(t_token *node, t_lists *lists) */
/* { */
/*     int fd; */
/*     char *limiter; */
/*     pid_t pid; */
/*     struct sigaction sa_orig_int, sa_orig_quit; */
/*      */
/*     setup_signal_handlers(&sa_orig_int, &sa_orig_quit); */
/*     fd = prepare_heredoc(node, &limiter); */
/*     if (fd == -1) */
/*         return (false); */
/*     setup_here_doc_parent_signals(); */
/*     pid = fork(); */
/*     if (pid < 0) */
/*       return (exec_here_doc_fork_error(fd, limiter, &sa_orig_quit, &sa_orig_int)); */
/*     if (pid == 0) */
/*         handle_child_process(fd, limiter, lists); */
/*     else */
/*     { */
/*         if (!handle_parent_process(pid, lists, &sa_orig_int, &sa_orig_quit)) */
/*         { */
/*             close(fd); */
/*             free(limiter); */
/*             return (false); */
/*         } */
/*     } */
/*     close(fd); */
/*     free(limiter); */
/*     return (true); */
/* } */


/* bool	exec_here_doc(t_token *node, t_lists *lists, bool *sig_hd) */
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
/*     signal(SIGINT, SIG_DFL); //ferme les hd un par un pour le moment */
/*     signal(SIGQUIT, SIG_IGN); */
/* 		close_origin_fds(lists->origin_fds); */
/* 		free_lists(lists); */
/* 		if (!extract_stdin(fd, limiter)) */
/* 			exit(EXIT_FAILURE); */
/* 		close(fd); */
/* 		exit(EXIT_SUCCESS); */
/* 	} */
/* 	else */
/* 	{ */
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
/* 	close(fd); */
/* 	free(limiter); */
/* 	return (true); */
/* } */


