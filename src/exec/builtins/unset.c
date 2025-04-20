

#include "structs.h"
#include "exec.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int env_size(t_var *env)
{
	int i;
	t_var *tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	builtin_unset(t_var **env, char **arg)
{
	t_var	*tmp;
	int		i;

	i = 1;
	while (arg[i])
	{
		tmp = *env;
		while (tmp)
		{
			if (!ft_strncmp(arg[i], tmp->key, ft_strlen(arg[i])))
			{
				if (env_size(*env) == 1)
				{
					free_list(env);
					*env = NULL;
					tmp = NULL;
					break ;
				}
				if (tmp->next)
					*env = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break ;
			}
			else
				tmp = tmp->next;
		}
		if (*env == NULL)
			break ;
		i++;
	}
	return (0);
}

/* Remove each variable or function name. If the -v option is given,
	each name */
/* refers to a shell variable and that variable is removed. If the
	-f option is given, */
/* the names refer to shell functions,
	and the function definition is removed. If */
/* the -n option is supplied, and name is a variable with the nameref attribute,
	*/
/* name will be unset rather than the variable it references.
	-n has no effect if */
/* the -f option is supplied. If no options are supplied,
	each name refers to a */
/* variable; if there is no variable by that name, a function with that name,
	if */
/* any,
	is unset. Readonly variables and functions may not be unset. Some shell */
/* variables lose their special behavior if they are unset; such behavior is noted in */
/* the description of the individual variables. The return status is zero unless a */
/* name is readonly or may not be unset. */
