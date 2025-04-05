

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

int builtin_export(t_env **env, char **arg)
{
	t_env *tmp;
	char *s;
	int i;
	char **line;
	
	s = NULL;
	line = NULL;
	tmp = *env;
	//LS_COLOR et la ligne longue disparait ??
	sort_env(&tmp);
	if (!arg[1]) //export sans arguments affiche l'env avec declare + x au debut de chaque lignes 
	{
		while (tmp)
		{
			line = ft_split(tmp->line, '=');
			/* if (line[1]) comment ca se pass si PWD= ? */
			s = build_line(NULL, "declare -x ", line[0], "=\"", line[1], "\"\n", NULL);
			ft_putstr_fd(s, 1);
			tmp = tmp->next;
			free(s);
			s = NULL;
			ft_free(line);
			line = NULL;
		}
		if (s)
			free(s);
		if (line)
			ft_free(line);
		return (0);
	}
	else 
	{
		i = 1;
		while (arg[i])
		{
			add_back(env, arg[i]);
			i++;
		}
	}
	while(tmp->next)
		tmp = tmp->next;
	//ajouter un maillon a env, avec la string var
	return (0);
}
