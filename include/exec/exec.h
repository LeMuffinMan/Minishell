/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:31:45 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 17:00:37 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "tree.h"
#include <sys/wait.h>
#include "structs.h"

//exec.c
int exec_ast (t_tree **ast, t_var **env, int origin_fds[2]);
int	exec_cmd(t_tree **ast, t_var **env, int origin_fds[2]);
// int redirect_stdio(t_tree **ast, t_var **env, int origin_fds[2]);
// int	exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes, int origin_fds[2]);
// int	builtins(char **arg, t_var **env, t_tree **ast, int origin_fds[2]);

//redirect_stdio_utils
int file_check(char *file, t_type type);
int is_a_directory(char *name);
int open_dup2_close(char *file, t_type type);

//pipe_utils
int free_pipes(t_pipe **pipes);
int add_pipe(int fd[2], t_pipe **pipes);
int	wait_children(pid_t last_child, pid_t first_child);

//errors.c
int error_cmd(char *cmd, int error_code);
int print_error_file_opening(char *file, char *error);
int error_not_valid_identifier(char *s);

t_tree	*parse(char *line, char **envp);

#endif
