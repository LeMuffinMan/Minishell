
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

//ne pas afficher les variables non exportees ? ou justement on affiche tout ?
//NE PAS AFFICHER LES VARIABLES SANS VALEUR !
int builtin_env(t_var **env)
{
	t_var *tmp;

	if (!*env)
	  return (0); // 0 ?
	tmp = *env;
	while (tmp)
	{
	  if (tmp->value && tmp->env == 1)
	  {
			ft_putstr_fd(tmp->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
