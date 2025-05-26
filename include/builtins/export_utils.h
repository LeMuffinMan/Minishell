
#ifndef EXPORT_UTILS_H
# define EXPORT_UTILS_H

#include "structs.h"

char *get_full_variable_declaration(char **arg, int i, char **s);
int is_var_exportation(char *s);
int is_var_declaration(char *arg);
int export_without_argument(t_var **env);

#endif
