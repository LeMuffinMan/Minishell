

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"


//DECIDER
//CAR=CAR
//export CAR 
//
//declare -x uniquement ?
//
//

//ne pas afficher TOUTES les variables
//afficher _= si on fait un bash -c export | grep ?
//certaines variables sont marquees comme exportees ?
//export affiche toutes les variables exportees
//_ est pas une variable mais listee comme un charactere special 
//la commande set renvoie : _=env
//Faire un booleen dans env pour marquer les variables exportees et les autres ? 
//differencier env grep _= / echo $_
//
//Doc bash : 
// At shell startup set to the pathname used to invoke the shell or shell script being executed as passed in the environment or argument list
// + Expands to the last argument to the previous simple command executed in the foreground after expansion.
// Also set to the full pathname used to invoke each command executed and placd in the environment exported to that command
// When checking mailm this parameter holds the name of the mail file
// Si je fais VAR=var puis export : elle n'apparait pas MAIS echo $VAR fonctionne !
// Si je fais ENSUITE export VAR : elle apparait dans l'export 
//
//INTERDIRE l'export de _ !!
//
//
//IL FAUT FAIRE UNE LISTE DE TOUS LES VAR=var fait en vrac
//     Ne pas les lister avec export sans args
//     excepte si on a fait export VAR
//
//


//on trie la liste
//on join les key avec un bool 1 et des \n en une line

int envsize(t_env *env)
{
	int i;
	t_env *tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

//probleme ici : cree des fuites 
int copy_node(t_env **dest, t_env **src)
{
    if (!*src || !*dest)
        return (1);
    (*dest)->key = NULL;
    (*dest)->value = NULL;
    (*dest)->next = NULL;
    if ((*src)->key)
        (*dest)->key = ft_strdup((*src)->key);
    if ((*src)->value)
        (*dest)->value = ft_strdup((*src)->value);
    (*dest)->env = (*src)->env;
    (*dest)->exported = (*src)->exported;
    return (0);
}









/* char *build_export_output(t_env **env) */
/* { */
/* 	t_env *copy; */
/* 	t_env *to_free; */
/* 	char *line; */
/**/
/* 	copy = NULL; */
/* 	line = ft_strdup(""); */
/* 	copy_list(&copy, env); */
/* 	sort_list(&copy); */
/* 	to_free = copy; */
/* 	while (copy) */
/* 	{ */
/* 		line = ft_strjoin(line, "declare -x ", line); */
/* 		line = ft_strjoin(line, copy->key, line); */
/* 		if (copy->value) */
/* 		{ */
/* 			line = ft_strjoin(line, "=", line); */
/* 			line = ft_strjoin(line, "\"", line); */
/* 			line = ft_strjoin(line, copy->value, line); */
/* 			line = ft_strjoin(line, "\"\n", line); */
/* 		} */
/* 		else  */
/* 			line = ft_strjoin(line, "\n", line); */
/* 		copy = copy->next; */
/* 	} */
/* 	free_list(&to_free);	 */
/* 	return (line); */
/* } */

int longest_strings_len(char *s1, char *s2)
{
	if (ft_strlen(s1) > ft_strlen(s2))
		return (ft_strlen(s1));
	else
		return (ft_strlen(s2));
}

t_env *is_known_key(t_env **env, char *key)
{
	t_env *tmp;
	
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, longest_strings_len(key, tmp->key)))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

//Sans arguments : 
//- on affiche les variables d'env (pas toutes ?)
//- on les trie par ordre alphabetique
//
//Avec un arg :
//- Si on a un VAR=var
//   - si la Variable existe mais n'est pas exportee, on set son bool export / env sur 1 ET on modifie sa valeur
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
//Test=rger
//export test=
//export VAR=5
//change pas test
//Pareil pour chaque arg
//

#include <stdio.h>
int compare_keys(char *key1, char *key2)
{
	char *longest_key;

	if (ft_strlen(key1) > ft_strlen(key2))
		longest_key = key1;
	else
		longest_key = key2;
	return (ft_strncmp(key1, key2, ft_strlen(longest_key)));
}


int swap_nodes(t_env *n1, t_env *n2)
{
	char *tmp_key;
	char *tmp_value;
	int tmp_env;
	int tmp_exported;

	tmp_key = n1->key;
	tmp_value = n1->value;
	tmp_env = n1->env;
	tmp_exported = n1->exported;

	n1->key = n2->key;
	n1->value = n2->value;
	n1->env = n2->env;
	n1->exported = n2->exported;

	n2->key = tmp_key;
	n2->value = tmp_value;
	n2->env = tmp_env;
	n2->exported = tmp_exported;

	return (0);
}

int sort_list(t_env **l)
{
	t_env *tmp;
	int sorted;

	sorted = 0;
	tmp = *l;
	if (!tmp)
		return (1);
	while(sorted == 0)
	{
		sorted = 1;
		tmp = *l;
		while (tmp->next)
		{
			if (compare_keys(tmp->key, tmp->next->key) > 0)
			{
				sorted = 0;
				swap_nodes(tmp, tmp->next);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

t_env *copy_list(t_env **env)
{
	t_env *tmp;
	t_env *last;
	t_env *new_node;
	t_env *copy;

	tmp = *env;
	last = NULL;
	copy = NULL;
	while (tmp)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			exit (1); // a mieux protegetr 
		new_node->key = ft_strdup(tmp->key);
		new_node->value = ft_strdup(tmp->value);
		new_node->next = NULL;
		if (last)
			last->next = new_node;
		else
			copy = new_node;	
		last = new_node;
		tmp = tmp->next;
	}
	return (copy);
}

int builtin_export(t_env **env, char **arg)
{
	t_env *node;
	t_env *copy;
	char **key_value;
	char *export_output;

	node = NULL;
	export_output = NULL;
	copy = NULL;
	if (!arg[1])
	{
		copy = copy_list(env);
		sort_list(&copy);
		if (copy)
			print_env(&copy);
		free_list(&copy);
		//declare -x KEY = "VALUE"
		//copy env list
		//bublesort env list
		//build line
		/* export_output = build_export_output(env); */
		/* ft_putstr_fd(export_output, 1); */
		/* free(export_output); */
	}
	else if (!ft_strncmp("_=", arg[1], 3))
		return(1);
	else
	{
		key_value = ft_split(arg[1], '=');
		node = is_known_key(env, key_value[0]);
		ft_free(key_value);
		//Si on a deja la cle dans les variables non visibles, on la rend visible
		if (node)
		{
			node->exported = 1;
			node->env = 1;
			return (0);
		}
		//sinon on l'ajoute et on la rend visible
		add_back(env, arg[1], 3); //changer le systene de mode ? 
	}
	return (0);
}



