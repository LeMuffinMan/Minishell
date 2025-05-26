
#ifndef EXEC_CMD_H
# define EXEC_CMD_H

int exec_cmd_print_error(t_tree **ast);
int missing_file_error(char **s, char *tmp);
int directory_error(char **s, char *tmp);
int exec_cmd_print_error(t_tree **ast);
int		is_a_directory(char *name);
int		exec_cmd_execve_child(t_tree **ast, t_lists *lists);

#endif
