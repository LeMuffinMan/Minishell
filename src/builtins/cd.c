
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

char *get_home_path(t_env **env)
{
	t_env *tmp;
	char *path;

	tmp = *env;
	path = NULL;
	while (tmp)
	{
		if (!ft_strncmp("HOME=", tmp->line, 5) && ft_strlen(tmp->line) > 5)
		{
			path = tmp->line + 5;
			break ;
		}
		tmp = tmp->next;
	}
	return (path);
}

//ajouter l'acces a des dossiers non permis ?
//cd dans un fichier ?
int	builtin_cd(char **arg, t_env **env)
{
	char	*s;

	/* char *new_path; */
	/* char	buf[PATH_MAX]; */
	/* new_path = NULL; */
	if (!arg[1])
	{
		s = get_home_path(env); // revoir ca
		if (!s)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 1);
			return (1);
		}
		else
			chdir(s);
		return (0);
	}
	s = ft_strdup("");
	if (arg[1][0] && arg[1][0] == '/') // Si on chere cd ~ on doit adapter ca
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
		else
		{
			s = ft_strjoin(s, "minishell: cd: ", s);
			s = ft_strjoin(s, arg[1], s);
			s = ft_strjoin(s, ": No such file or directory\n", s);
			ft_putstr_fd(s, 2);
			free(s);
			return (1);
		}
	}
	else if (arg[1][0]) // on est sur un chemin relatif 
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
	}
	/* free(new_path); */
	free(s);
	return (0);
}
