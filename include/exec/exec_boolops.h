
#ifndef EXEC_BOOLOPS_H
# define EXEC_BOOLOPS_H

#include "structs.h"

int exec_boolop(t_tree **ast, t_lists *lists);
int  exec_parenthesis_child(t_tree **ast, t_lists *lists);
int exec_parenthesis(t_tree **ast, t_lists *lists);
int exec_group_cmd(t_tree **ast, t_lists *lists);
int exec_group_boolop(t_tree **ast, t_lists *lists);

#endif
