

#include "structs.h"
#include "exec.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

// DECIDER
// CAR=CAR
// export CAR
//
// declare -x uniquement ?
//
//

// ne pas afficher TOUTES les variables
// afficher _= si on fait un bash -c export | grep ?
// certaines variables sont marquees comme exportees ?
// export affiche toutes les variables exportees
//_ est pas une variable mais listee comme un charactere special
// la commande set renvoie : _=env
// Faire un booleen dans env pour marquer les variables exportees et les autres ?
// differencier env grep _= / echo $_
//
// Doc bash :
// At shell startup set to the pathname used to invoke the shell or shell script being executed as passed in the environment or argument list
//
	/* + Expands to the last argument to the previous simple command executed in the foreground after expansion. */
// Also set to the full pathname used to invoke each command executed and placd in the environment exported to that command
// When checking mailm this parameter holds the name of the mail file
// Si je fais VAR=var puis export : elle n'apparait pas MAIS echo $VAR fonctionne !
// Si je fais ENSUITE export VAR : elle apparait dans l'export
//
// INTERDIRE l'export de _ !!
//
//
// IL FAUT FAIRE UNE LISTE DE TOUS LES VAR=var fait en vrac
//     Ne pas les lister avec export sans args
//     excepte si on a fait export VAR
//
//

// on trie la liste
// on join les key avec un bool 1 et des \n en une line

// Sans arguments :
//- on affiche les variables d'env (pas toutes ?)
//- on les trie par ordre alphabetique
//
// Avec un arg :
//- Si on a un VAR=var
//   - si la Variable existe mais n'est pas exportee, on set son bool export
	/* / env sur 1 ET on modifie sa valeur */
//   - si la variable n'existe pas on l'export direct
//   - si la variable est deja exportee, on modifie sa valeur
//
//- si on a VAR
//  - si on a deja un VAR=erger : on l'export avec sa valeur
//  - si on a pas de variable deja declaree, on l'export sans valeur
//
// - si on a VAR=
// 	- si on a deja VAR ??
// 	- si on l'a pas, on lui donne "" pour valeur
//
// Test=rger
// export test=
// export VAR=5
// change pas test
// Pareil pour chaque arg
//

int free_array(char **array)
{
	int i;
	
	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}

t_var	*is_known_key(t_var **env, char *key)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	compare_keys(char *key1, char *key2)
{
	char	*longest_key;

	if (ft_strlen(key1) > ft_strlen(key2))
		longest_key = key1;
	else
		longest_key = key2;
	return (ft_strncmp(key1, key2, ft_strlen(longest_key)));
}

int	builtin_export(t_var **env, char **arg)
{
	t_var	*node;
	t_var	*copy;
	char	**key_value;

	(void)key_value;
	node = NULL;
	copy = NULL;
	if (!arg[1])
	{
		copy = copy_list(env);
		print_sorted_env(&copy);
		//a reparer
		/* sort_list(&copy); */
		/* if (copy) */
		/* { */
		/* print_sorted_env(&copy); */
		/* 	free_list(&copy); */
		/* } */
	}
	else if (!ft_strncmp("_=", arg[1], 3))
		return (0);
	else
	{
		key_value = ft_split(arg[1], '=');
		node = is_known_key(env, key_value[0]);
		// Si on a deja la cle dans les variables non visibles,
			/* on la rend visible */
		if (node)
		{
			if (node->value)
				free(node->value);
			if (key_value && key_value[1])
				node->value = ft_strdup(key_value[1]);
			else 
				node->value = ft_strdup(""); // pas suuuur
			//pas toujours !
			node->exported = 1;
			node->env = 1;
			return (0);
		}
		// sinon on l'ajoute et on la rend visible
		add_back_var(env, arg[1], 3); // changer le systene de mode ?
		free_array(key_value);
	}
	return (0);
}
