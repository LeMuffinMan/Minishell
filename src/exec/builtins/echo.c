
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

// DECIDER
//
//- parsing clean les -n dupliques ?
//- histoire du \n du prompt ?

// Si echo $_
// 	on veut afficher le dernier argument !
	// booleen pour l'option ?
	// Si on a comme 1er param le -n : skip pour trouver des str a afficher
	// si on a deja un -n, on skip tout ceux qui suivent
	// on join tous les args avec un space entre chaque, sauf le dernier
	// si on n'avait pas de -n : on join le \n
	// si on dup2 au tout debut, ici 1 ecrira bien ou je vuex ?
int	builtin_echo(char **arg)
{
	int		option;
	int		i;
	char	*s;
	char	*tmp;

	option = 0;
	if (!arg[1])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	s = ft_strdup("");
	i = 1;
	if (!ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[i]) == 2)
	{
		option = 1;
		i++;
	}
	if (option == 1)
	{
		while (arg[i] && !ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[1]) == 2)
			i++;
	}
	while (arg[i])
	{
		tmp = s;
		s = ft_strjoin(s, arg[i]);
		free(tmp);
		if (arg[i + 1])
		{
			tmp = s;
			s = ft_strjoin(s, " ");
			free(tmp);
		}
		i++;
	}
	if (option == 0)
	{
		tmp = s;
		s = ft_strjoin(s, "\n");
		free(tmp);
	}
	if (s)
		ft_putstr_fd(s, 1);
	free(s);
	return (0);
}
