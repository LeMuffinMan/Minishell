
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>


//REVOIR BUILD LINE !
// tests :
// echo
// echo -n
// echo -n -n -n
// echo -n hello
// echo -n -n hello
// echo hello
int	builtin_echo(char **arg)
{
	int		option;
	int		i;
	char	*s;

	// booleen ?
	s = ft_strdup("");
	option = 0;
	if (!arg[1])
	{
		free(s);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	i = 1;
	// Si on a comme 1er param le -n : skip pour trouver des str a afficher
	if (!ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[i]) == 2)
	{
		option = 1;
		i++;
	}
	// si on a deja un -n, on skip tout ceux qui suivent
	if (option == 1)
	{
		while (arg[i] && !ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[1]) == 2)
			i++;
	}
	// on join tous les args avec un space entre chaque, sauf le dernier
	while (arg[i])
	{
		s = ft_strjoin(s, arg[i], s);
		if (arg[i + 1])
			s = ft_strjoin(s, " ", s);
		i++;
	}
	// si on n'avait pas de -n : on join le \n
	if (option == 0)
		s = ft_strjoin(s, "\n", s);
	// si on dup2 au tout debut, ici 1 ecrira bien ou je vuex ?
	if (s)
		ft_putstr_fd(s, 1);
	free(s);
	return (0);
}


