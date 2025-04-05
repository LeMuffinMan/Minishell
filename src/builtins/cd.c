
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

//REVOIR BUILD LINE !
//ajouter l'acces a des dossiers non permis ?
//cd dans un fichier ?
int	builtin_cd(char **arg)
{
	char	*s;

	/* char *new_path; */
	/* char	buf[PATH_MAX]; */
	/* new_path = NULL; */
	s = NULL;
	if (!arg[1])
	{
		// si HOME existe : on chdir(HOME)
		// si non :
		ft_putstr_fd("minishell: cd: HOME not set", 1);
		return (1);
	}
	if (arg[1][0] && arg[1][0] == '/') // Si on chere cd ~ on doit adapter ca
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
		else
		{
			s = build_line("minishell: cd: ", arg[1], s);
			s = build_line(s, ": No such file or directory\n", s);
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
	return (0);
}
