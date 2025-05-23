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
#include "env_utils.h"

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
		if (!key_value[1])
			key_value[1] = ft_strdup("");
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
			  (!ft_strncmp(tmp->key, "PS1", ft_strlen(tmp->key) + 1) && !ft_strncmp(key, tmp->key, ft_strlen(tmp->key)))) // A VIRERRRRR !!!!!!!!
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int is_var_declaration(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '=' || ft_isdigit(arg[i]) || !ft_isalnum(arg[i]))
		return (0);
	while (arg[i])
	{
		if (arg[i] == '+' && arg[i + 1] && arg[i + 1] != '=')
			return (0);
		if (!ft_isalnum(arg[i]))
		{
			if ((arg[i] == '=') || (arg[i + 1] && arg[i] == '+' && arg[i + 1] == '='))
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

int is_an_incrementation(char *arg)
{
	int i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=' && arg[i - 1] == '+')
			return (1);
		i++;
	}
	return (0);
}

char *clean_inc_operator(char *arg)
{
	int i;
	int j;
	char *s;

	/* #include <stdio.h> */
	/* printf("arg = %s\n", arg); */
	s = malloc(sizeof(char) * (ft_strlen(arg)));
	if (!s)
	{
		//malloc error
	}
	i = 0;
	while (arg[i] && arg[i] != '+')
	{
		s[i] = arg[i];
		i++;
	}
	if (arg[i] == '+')
	{
		s[i] = '=';
		j = i + 1;
		i+=2;
	}
	else
	{
		s[i] = '\0';
		return (s);
	}
	while(arg[i])
	{
		s[j] = arg[i];
		j++;
		i++;
	}
	s[j] = '\0';
	/* printf("s = %s\n", s); */
	return (s);
}


char *get_full_variable_declaration(char **arg, int i)
{
	char *s;

	if (arg[i][ft_strlen(arg[i]) - 1] == '=') //si s finit par = 
	{
		if (arg[i + 1] && !is_var_declaration(arg[i + 1])) //et que content[i + 1] est sa valeur
			s = ft_strjoin(arg[i], arg[i + 1]);
		else
			s = ft_strdup(arg[i]); //si on a une declaration de var en content[i + 1]
	}
	else //si on a un cas classique
		s = ft_strdup(arg[i]);
	return (s);
}

int add_or_update_var(t_var **env, char *var)
{
	int inc;
	char *s;
	char *tmp;
	char *value;
	char **key;
	t_var *node;

	inc = 0;
	if (is_an_incrementation(var))
	{
		/* #include <stdio.h> */
		/* if (!ft_strncmp("SHLVL", var, ft_strlen(var))) */
		/* 	printf("%s\n", var); */
		s = clean_inc_operator(var);
		inc = 1;
	}
	else
		s = ft_strdup(var);
	key = ft_split(s, '='); //surement qu'on peut faire plus simple au'un split
	node = is_known_exported_key(env, key[0]);
	free_array(key);
	if (node) // si on l'a deja
	{
		value = ft_strchr(var, '=');
		if (inc)
		{
			tmp = node->value;
			node->value = ft_strjoin(node->value, value + 1);
			free(tmp);
		}
		else
		{
			free(node->value);
			node->value = ft_strdup(value + 1);
		}
	}
	else // si on l'a pas 
		add_back_var(env, s, 3);
	if (s)
		free(s);
	return (0);
}

int is_var_exportation(char *s)
{
	int i;

	i = 0;
	if (s[i] == '=' || ft_isdigit(s[i]) || !ft_isalnum(s[i]))
		return (0);
	while(s[i + 1])
	{
		if (s[i] == '+' && s[i + 1] && s[i + 1] != '=')
			return (0);
		if (s[i] < '0')
			return (0);
		i++;
	}
	if (s[i] == '=')
		return (0);
	return (1);
}

//PS1 to fix
int	builtin_export(t_var **env, char **arg)
{
	t_var *node;
	int i;
	char *s;
	int exit_code;

	i = 1;
	if (!arg[i])
		return(export_without_argument(env));
	exit_code = 0;
	while(arg[i])
	{
		if (is_var_declaration(arg[i])) //si la declaration de variable est vlaide
		{
			s = get_full_variable_declaration(arg, i);
			add_or_update_var(env, s);
			free(s);
		}
		else if (is_var_exportation(arg[i]))
		{
			node = is_known_key(env, arg[i]);
			if (node)
				node->exported = 1;
		}
		else
		{
			exit_code = 1;
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		}
		i++;
	}
	return (exit_code);
}
