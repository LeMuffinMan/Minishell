
#ifndef PARSE_AND_EXECUTION_LOOP_H
# define PARSE_AND_EXECUTION_LOOP_H

#include "structs.h"

int	parse_and_execution_loop(char **env, char **prompt, t_lists *lists,
		int exit_code);
int	init(t_lists *lists, char **av, char **env);
int		exec_ast(t_tree **ast, t_lists *lists);
t_tree	*parse(char *line, t_var *new_env, t_lists *lists);
int	malloc_error_close_free_exit(t_lists *lists);
void free_lists(t_lists *lists);
int	close_origin_fds(int origin_fds[2]);
void collect_signal_exit_code(t_lists *lists);
int	is_interactive_mode(void);

#endif

