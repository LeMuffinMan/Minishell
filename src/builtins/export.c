

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

/* int is_duplicated_var(char *var, t_env **env) */
/* { */
/* 	t_env *tmp; */
/**/
/* 	tmp = *env; */
/* 	while (tmp) */
/* 	{ */
/* 		if (!ft_strncmp(var, tmp->line, ft_strlen(tmp->line))  */
/* 			&& ft_strlen(var) == ft_strlen(tmp->line)) */
/* 			return (1); */
/* 		tmp = tmp->next; */
/* 	} */
/* 	return (0); */
/* } */
/**/
/* int equal_checker(char *s) */
/* { */
/* 	int i; */
/* 	int count; */
/**/
/* 	i = 0; */
/* 	count = 0; */
/* 	while (s && s[i]) */
/* 	{ */
/* 		if (s[i] == '=') */
/* 			count++; */
/* 		i++; */
/* 	} */
/* 	if (count == 1) */
/* 		return (0); */
/* 	else */
/* 		return (1); */
/* } */

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

/* char **lst_to_array(t_env **env) */
/* { */
/* 	t_env *tmp; */
/* 	char **array; */
/* 	char *s; */
/* 	int i; */
/**/
/* 	i = 0; */
/* 	s = NULL; */
/* 	array = (sizeof(char*) * ft_lstsize(env) + 1); */
/* 	tmp = *env; */
/* 	while (tmp) */
/* 	{ */
/* 		s = ft_strjoin(tmp->key, "=\"", s); */
/* 		s = ft_strjoin(s, tmp->value, s); */
/* 		s = ft_strjoin(s, "\"", s); */
/* 		tmp = tmp->next; */
/* 		i++; */
/* 	} */
/* 	array[i] = NULL; */
/* } */

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

//a tester 
char *build_export_output(t_env **env)
{
	int i;
	t_env *tmp;
	t_env *min_node;
	t_env *last_node;
	char *line;
	char *longest_key;

	i = 0; 
	line = NULL;
	min_node = *env;
	tmp = *env;
	while (i < envsize(*env))
	{
		while (tmp)
		{
			if (ft_strlen(tmp->key) > ft_strlen(min_node->key))
				longest_key = tmp->key;
			else 
				longest_key = min_node->key;
			/* si on a trouve un node plus petit que notre min node */
			if (ft_strncmp(tmp->key, min_node->key, ft_strlen(longest_key)) < 0) 
			{
				if (ft_strlen(tmp->key) > ft_strlen(last_node->key)) 
					longest_key = tmp->key;
				else
					longest_key = last_node->key;
					/* et qu'il est aussi superieur au dernier node ajoute*/
				if (ft_strncmp(tmp->key, last_node->key, ft_strlen(longest_key) > 0))  
					min_node = tmp;
			}
			tmp = tmp->next;
		}
		last_node = min_node;
		if (tmp->exported == 1)
			line = build_line(line, "declare -x ", tmp->key, "=\"", tmp->value, "\"\n", NULL);
		i++;
	}
	return (line);

	//on cherche le plus petit de toutes les key : pas de doublons
		//si il est sur un bool exported
			//on join toute la ligne 
	//on cherche le plus petit plus grand que le precedent 
}

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

int builtin_export(t_env **env, char **arg)
{
	t_env *node;
	char **key_value;
	char *export_output;

	node = NULL;
	export_output = NULL;
	if (!arg[1])
	{
		//declare -x KEY = "VALUE"
		export_output = build_export_output(env);
		ft_putstr_fd(export_output, 1);
		free(export_output);
	}
	else if (!ft_strncmp("_=", arg[1], 2))
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



/**/
/**/
/* int builtin_export(t_env **env, char **arg) */
/* { */
/* 	t_env *tmp; */
/* 	char *s; */
/* 	int i; */
/* 	char **line; */
/* 	 */
/* 	s = NULL; */
/* 	line = NULL; */
/* 	tmp = *env; */
/* 	//LS_COLOR et la ligne longue disparait ?? */
/* 	sort_env(&tmp); */
/* 	if (!arg[1]) //export sans arguments affiche l'env avec declare + x au debut de chaque lignes  */
/* 	{ */
/* 		while (tmp) */
/* 		{ */
/* 			line = ft_split(tmp->line, '='); */
/* 			//declare -x toujours ? */
/* 			s = build_line(NULL, "declare -x ", line[0], "=\"", line[1], "\"\n", NULL); */
/* 			ft_putstr_fd(s, 1); */
/* 			tmp = tmp->next; */
/* 			free(s); */
/* 			s = NULL; */
/* 			ft_free(line); */
/* 			line = NULL; */
/* 		} */
/* 		if (s) */
/* 			free(s); */
/* 		if (line) */
/* 			ft_free(line); */
/* 		return (0); */
/* 	} */
/* 	else  */
/* 	{ */
/* 		i = 1; */
/* 		while (arg[i]) */
/* 		{ */
/* 			//a tester le equal_checker  */
/* 			if (!is_duplicated_var(arg[i], env) && !equal_checker(arg[i])) */
/* 				add_back(env, arg[i]); */
/* 			i++; */
/* 		} */
/* 	} */
/* 	return (0); */
/* } */
