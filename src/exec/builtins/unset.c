/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:33:13 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:33:31 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "minishell.h"
#include "structs.h"
#include <stdlib.h>

int	env_size(t_var *env)
{
	int		i;
	t_var	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

//return 0 si a trouve le node a free
//return 1 si il n'a pas trouve de node a free
/* int delete_node(t_var **node, t_var **env) */
/* { */
/* 	t_var *tmp; */
/* 	t_var *last; */
/* 	 */
/* 	last = NULL; */
/* 	if (env_size(*env) == 1) */
/* 	{ */
/* 		free((*env)->key); */
/* 		free((*env)->value); */
/* 		free(*env); */
/* 		env = NULL; */
/* 		return (0); */
/* 	} */
/* 	tmp = *env; */
/* 	while(tmp) */
/* 	{ */
/* 		if (tmp == *node) */
/* 		{	 */
/* 			if (last) */
/* 				last->next = tmp->next; */
/* 			free(node->key); */
/* 			free(node->value); */
/* 			free(tmp); */
/* 			return (0); */
/* 		} */
/* 		last = tmp; */
/* 		tmp = tmp->next; */
/* 	} */
/* 	return (1); */
/* } */

int builtin_unset(t_var **env, char **arg)
{
	t_var	*tmp;
	/* t_var *node_to_remove; */
	int i;

	i = 1;
	while(arg[i])
	{
		tmp = *env;
		while(tmp)
		{
			if (tmp->key && !ft_strncmp(tmp->key, arg[i], ft_strlen(arg[i])))
			{
				/* node_to_remove = tmp; */
				tmp = tmp->next;
				/* delete_node(&node_to_remove, env); */ //a revoir ! des invalid read of size
				return (0);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

/* int	builtin_unset(t_var **env, char **arg) */
/* { */
/* 	t_var	*tmp; */
/* 	t_var	*temp; */
/* 	int		i; */
/**/
/* 	i = 1; */
/* 	while (arg[i] && *env) */
/* 	{ */
/* 		tmp = *env; */
/* 		while (tmp) */
/* 		{ */
/* 			if (tmp->next!ft_strncmp(arg[i], tmp->next->key, ft_strlen(arg[i]))) */
/* 			{ */
/* 				temp = tmp;	 */
/* 				tmp = tmp->next; */
/* 				temp->next = tmp->next; */
/* 				if (env_size(*env) == 1) */
/* 				{ */
/* 					free_list(env); */
/* 					*env = NULL; */
/* 					tmp = NULL; */
/* 					break ; */
/* 				} */
/* 				free(tmp->key); */
/* 				free(tmp->value); */
/* 				free(tmp); */
/* 				break ; */
/* 			} */
/* 			else */
/* 				tmp = tmp->next; */
/* 		} */
/* 		if (*env == NULL) */
/* 			break ; */
/* 		i++; */
/* 	} */
/* 	return (0); */
/* } */
/**/
/* Remove each variable or function name. If the -v option is given,
	each name */
/* refers to a shell variable and that variable is removed. If the
	-f option is given, */
/* the names refer to shell functions,
	and the function definition is removed. If */
/* the -n option is supplied, and name is a variable with the nameref attribute,
 */
/* name will be unset rather than the variable it references.
	-n has no effect if */
/* the -f option is supplied. If no options are supplied,
	each name refers to a */
/* variable; if there is no variable by that name, a function with that name,
	if */
/* any,
	is unset. Readonly variables and functions may not be unset. Some shell */
/* variables lose their special behavior if they are unset; such behavior
 * is noted in */
/* the description of the individual variables. The return status is zero
 * unless a */
/* name is readonly or may not be unset. */
