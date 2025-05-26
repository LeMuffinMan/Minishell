
#ifndef EXEC_PIPE_H
# define EXEC_PIPE_H

#include "tree.h"
#include "structs.h"

int		exec_pipe(t_tree **ast, t_lists *lists);
int		handle_right_execution(t_tree **ast, t_lists *lists, pid_t left_pid,
			struct sigaction *sa_orig);
int		exec_pipe_right_pipe_execution(t_tree **ast, t_lists *lists,
			pid_t left_pid);
int		exec_pipe_right_execution(t_tree **ast, t_lists *lists, pid_t right_pid,
			pid_t left_pid);
int		exec_pipe_left_execution(t_tree **ast, t_lists *lists, pid_t pid);
int	add_pipe(int fd[2], t_pipe **pipes);
int	left_child_execution(t_tree **ast, t_lists *lists);
int	right_child_execution(t_tree **ast, t_lists *lists, int pipefd[2],
		t_pipe **pipes);
int	left_parent_execution(t_pipe **pipes, int pipefd[2]);
int	free_pipes(t_pipe **pipes);
int setup_pipe_signals(struct sigaction *sa_ignore, struct sigaction *sa_orig);
void	free_lists(t_lists *lists);
int		close_origin_fds(int origin_fds[2]);
int		exec_ast(t_tree **ast, t_lists *lists);

#endif
