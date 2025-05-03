#ifndef ENV_UTILS_H
# define ENV_UTILS_H

#include "structs.h"

int	init_env(t_var **new_env, char **env, char *name_of_program);
void	add_back_var(t_var **lst, char *s, int mode);
void	add_first_node(t_var **lst, t_var **new, char *s, int mode);
char	*get_value(t_var **env, char *key);
int	update_env(t_var **env);
int update_last_arg_var(t_var **env, char **content);
t_var	*is_known_key(t_var **env, char *key);

#endif
