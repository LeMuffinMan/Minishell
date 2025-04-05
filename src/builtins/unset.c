

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>


int builtin_unset(t_env **env, char **arg)
{
	t_env *tmp;
	int i;

	i = 1;
	while (arg[i])
	{
		tmp = *env;
		while (tmp)
		{
			if (ft_strncmp(arg[i], tmp->line, ft_strlen(arg[i])) == 0 &&
    		tmp->line[ft_strlen(arg[i])] == '=')
				{
					if (tmp->prev)
						tmp->prev->next = tmp->next;
					if (tmp->next)
						tmp->next->prev = tmp->prev;
					free(tmp->line);
					free(tmp);
					break;
				}
				else
					tmp = tmp->next;
		}
		i++;
	}
	return (0);
}

