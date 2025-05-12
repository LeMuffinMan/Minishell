
#ifndef ALIASES_H
# define ALIASES_H 

#include "structs.h"

t_alias *is_a_known_alias(char *word, t_alias **aliases);
char *expand_alias(char **content, t_alias **alias);

#endif
