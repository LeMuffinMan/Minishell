/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:31:45 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/23 21:54:11 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"
# include "tree.h"
# include <sys/wait.h>

// exec.c
int		exec_ast(t_tree **ast, t_lists *lists);
int		exec_cmd(t_tree **ast, t_lists *lists);
int	builtins(char **arg, t_lists *lists);
// int redirect_stdio(t_tree **ast, t_var **env, int origin_fds[2]);
// int	builtins(char **arg, t_lists *lists, int origin_fds[2]);

// redirect_stdio_utils
int		file_check(char *file, t_type type, int error_code);
int		is_a_directory(char *name);
int		open_dup2_close(char *file, t_type type);

//exec_pipe.c
int	exec_pipe(t_tree **ast, t_lists *lists);
int	handle_right_execution(t_tree **ast, t_lists *lists, pid_t left_pid, 
	struct sigaction *sa_orig);
int exec_pipe_right_pipe_execution(t_tree **ast, t_lists *lists, pid_t left_pid);
int exec_pipe_right_execution(t_tree **ast, t_lists *lists, pid_t right_pid, pid_t left_pid);
int exec_pipe_left_execution(t_tree **ast, t_lists *lists, pid_t pid);

//exec_pipe_parent_child.c
int left_child_execution_stdio(t_lists *lists);
int left_child_execution(t_tree **ast, t_lists *lists);
int left_parent_execution(t_pipe **pipes, int pipefd[2]);
int right_child_execution(t_tree **ast, t_lists *lists, int pipefd[2], t_pipe **pipes);

//exec_cmd.c
int exec_cmd_execve_child(t_tree **ast, t_lists *lists);
int	exec_cmd(t_tree **ast, t_lists *lists);
int exec_cmd_execve(t_tree **ast, t_lists *lists);

//redirect_stdio.c


// pipe_utils
int		free_pipes(t_pipe **pipes);
int		add_pipe(int fd[2], t_pipe **pipes);
int		wait_children(pid_t last_child, pid_t first_child);

// errors.c
int		error_cmd(char *cmd, int error_code);
int		print_error_file_opening(char *file, char *error, int error_code);
int		error_not_valid_identifier(char *s);
int print_error_is_a_directory(char *file);
int print_perm_error(char *file);


int	redirect_stdio(t_tree **ast, t_lists *lists);
int exec_redirection(t_tree **ast, t_lists *lists);

t_tree	*parse(char *line, t_var *new_env, t_lists *lists);

#endif
