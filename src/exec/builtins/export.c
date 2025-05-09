/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:34:18 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:35:52 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "builtins.h"
#include "utils.h"

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
// Faire un booleen dans env pour marquer
// les variables exportees et les autres ?
// differencier env grep _= / echo $_
//
// Doc bash :
// At shell startup set to the pathname used to invoke the shell or shell
// script being executed as passed in the environment or argument list
//
/*
	+ Expands to the last argument to the previous simple command executed
	in the foreground after expansion. */
// Also set to the full pathname used to invoke each command executed
// and placd in the environment exported to that command
// When checking mailm this parameter holds the name of the mail file
// Si je fais VAR=var puis export :
// elle n'apparait pas MAIS echo $VAR fonctionne !
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

//verifier le unset d'une liste vide
//verifier PWD et OLDPWD
//verifier export / export arg args

// a reparer
/* sort_list(&copy); */
/* if (copy) */
/* { */
/* print_sorted_env(&copy); */
/* 	free_list(&copy); */
/* } */


int update_var(t_var **node, char **key_value, int inc)
{
	char *s;
	char *tmp;


	if (inc == 1)
	{
		s = ft_strdup((*node)->value);
		tmp = s;
		s = ft_strjoin(s, key_value[1]);
		free(tmp);
		free((*node)->value);	
		(*node)->value = s;
	}
	else
	{
		free((*node)->value);
		(*node)->value = ft_strdup(key_value[1]);
	}
	(*node)->exported = 1;
	return(0);
}

int export_without_argument(t_var **env)
{
	t_var *copy;

	copy = NULL;
	copy = copy_list(env);
	sort_list(&copy);
	print_sorted_env(&copy);
	free_list(&copy);
	return (0);
}

t_var *is_known_exported_key(t_var **env, char *key)
{
	t_var *tmp;

	tmp = *env;
	while(tmp)
	{
		if ((!ft_strncmp(tmp->key, key, ft_strlen(tmp->key) + 1) && tmp->exported == 1) ||
		(!ft_strncmp(tmp->key, "PS1", ft_strlen(tmp->key) + 1 && !ft_strncmp(key, "PS1", ft_strlen(key)))))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int add_or_update_var(t_var **env, char *arg)
{
	t_var *node;
	int inc;
	char **key_value;
	char *tmp;

	inc = 0;
	node = NULL;
	key_value = ft_split(arg, '='); // marche pas, je dois split au 1er egal seuelement
	if (!key_value)
		return (-1);
	/* if (!key_value[1]) // pas du tout sur de ce fix ! */
	/* 	key_value[1] = ft_strdup(""); */
	if (key_value [0] && key_value[1]) // si on a 3 words : on concatene les derniers mots
		key_value = concat_var(key_value);
	if (is_increment_operator(key_value[0]))
	{
		inc = 1; 
		key_value[0] = trim_operator(key_value[0]);
		tmp = key_value[1];
		key_value[1] = ft_strjoin("", arg + ft_strlen(key_value[0]) + 2);
		/* printf("key_value[1] = %s\n", key_value[1]); */
		free(tmp);
	}
	//il me faudrait un is_known_key specifique aux variables exported pour pas override _ ou ?
	node = is_known_exported_key(env, key_value[0]);
	/* if (node) */
	/* { */
	/* 	#include <stdio.h> */
	/* 	printf("%s=%s | exported = %d | env = %d\n", node->key, node->value, node->exported, node->env); */
	/* } */
	if (!node)
		add_new_var(env, key_value);
	else
		update_var(&node, key_value, inc);
	free_array(key_value);
	return (0);
}

// Si on a deja la cle dans les variables non visibles,
/* on la rend visible */
// pas suuuur (le strdup"")
// pas toujours exportee en 1 pour les deux !
// sinon on l'ajoute et on la rend visible
//  // changer le systene de mode ?
int	builtin_export(t_var **env, char **arg)
{
	int i;
	char *s;

	if (!arg[1])
		return(export_without_argument(env));
	else
	{
		i = 1;
		while(arg[i])
		{
			if (is_valid_identifier(arg[i]))
				error_not_valid_identifier(arg[i]);
			else
			{
				if (arg[i + 1])
					s = ft_strjoin(arg[i], arg[i + 1]);
				else
					s = ft_strjoin(arg[i], "");

				add_or_update_var(env, s);
				free(s);
			}
			i++;
		}
	}
	return (0);
}
