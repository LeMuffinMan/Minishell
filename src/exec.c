/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/03 17:57:58 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//et si on me declare des varialbes _=_ ?
//
//
//Attention : 
//echo $_ renvoie le dernier argument : echo; echo $_ : echo // echo hello; echo $_ : hello
/* int update_last_cmd_var(t_env **env, char *last_cmd) */
/* { */
/* 	t_env *tmp; */
/* 	char *s; */
/**/
/* 	tmp = *env; */
/* 	//on cherche _= */
/* 	s = ft_strdup("_="); */
/* 	s = ft_strjoin(s, last_cmd, s); */
/* 	//trouver le path pour la last cmd  */
/* 	//si elle a echoue, comment fait bash ? */
/* 	while (tmp) */
/* 	{ */
/* 		if (!ft_strncmp("_", tmp->key, ft_strlen(tmp_key))) */
/* 			break; */
/* 		tmp = tmp->next; */
/* 	} */
/* 	//si on est arrive au bout de la liste */
/* 	if (!tmp) */
/* 		add_back(env, s, 1);	 */
/* 	//si on a deja un _ on free la line et on la realloue */
/* 	else */
/* 	{ */
/* 		free(tmp->line); */
/* 		tmp->line = ft_strdup(s); */
/* 	} */
/* 	free(s); */
/* 	return (0); */
/* } */

int update_last_cmd_var(t_env **env, char *last_cmd)
{
	t_env *tmp;

	//la variable doit contenir le PATH !
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp("_", tmp->key, ft_strlen(tmp->key)))
			break;
		tmp = tmp->next;
	}
	/* if (!tmp) //ca ne devrait pas pouvoir etre possible */
	/* 	add_back(env, s, 1);	 */
	/* else */
	/* { */
		free(tmp->value);
		tmp->value = ft_strdup(last_cmd);
	/* } */
	return (0);
}

int	exec(char **arg, t_env **env)
{
	//a tester 
	if (arg[0])
		update_last_cmd_var(env, arg[0]);
	/* faire un switch case et y mettre les returns */
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 4))
		return(builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 2))
		return(builtin_cd(arg, env));
	else if (!ft_strncmp(arg[0], "pwd", 3))
		return(builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 6))
		return (builtin_export(env, arg));
	else if (!ft_strncmp(arg[0], "unset", 5))
		return(builtin_unset(env, arg));
	else if (!ft_strncmp(arg[0], "env", 3))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 4))
		return(builtin_exit(arg, env));
	else // pas printf ! utiliser fd_putstr_fd
		printf("child process will run any other cmd\n");
	return (0);
}

