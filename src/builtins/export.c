

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

int is_duplicated_var(char *var, t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->line, ft_strlen(tmp->line)) 
			&& ft_strlen(var) == ft_strlen(tmp->line))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

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
			if (!is_duplicated_var(arg[i], env))
				add_back(env, arg[i]);
			i++;
		}
	}
	//ajouter un maillon a env, avec la string var
	return (0);
}
