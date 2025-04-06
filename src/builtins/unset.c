

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

int	env_size(t_env *lst)
{
	t_env	*ptr;
	size_t	i;

	i = 0;
	ptr = lst;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

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
					if (env_size(*env) == 1)
					{
						free(tmp->line);	
						free(tmp);
						*env = NULL;
						tmp = NULL;
						break;
					}
					if (tmp->prev)
						tmp->prev->next = tmp->next;
					if (tmp->next)
					{
						*env = tmp->next;
						tmp->next->prev = tmp->prev;
					}
					free(tmp->line);
					free(tmp);
					break;
				}
				else
					tmp = tmp->next;
		}
		if (*env == NULL)
			break;
		i++;
	}
	return (0);
}

