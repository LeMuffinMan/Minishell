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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*ft_join(char *s1, char *s2, char *old_str)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_str == NULL)
		return (NULL);
	while (s1 && s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		new_str[i] = s2[j];
		i++;
		j++;
	}
	new_str[i] = '\0';
	free(old_str);
	return (new_str);
}

//tests : 
//echo 
//echo -n 
//echo -n -n -n 
//echo -n hello
//echo -n -n hello
//echo hello
int	builtin_echo(char **arg)
{
	int		option;
	int		i;
	char	*s;

	// booleen ?
	s = NULL;
	option = 0;
	if (!arg[1])
	{
		ft_putstr_fd("\n", 1);
		return(0);
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
		while (arg[i] && !ft_strncmp(arg[i], "-n", 2)
			&& ft_strlen(arg[1]) == 2)
			i++;
	}
	// on join tous les args avec un space entre chaque, sauf le dernier
	while (arg[i])
	{
		s = ft_join(s, arg[i], s);
		if (arg[i + 1])
			s = ft_join(s, " ", s);
		i++;
	}
	// si on n'avait pas de -n : on join le \n
	if (option == 0)
		s = ft_join(s, "\n", s);
	// si on dup2 au tout debut, ici 1 ecrira bien ou je vuex ?
	ft_putstr_fd(s, 1);
	free(s);
	return (0);
}

//tester en unsetant PWD ...
int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	char *s;

	s = NULL;
	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		s = ft_join(buf, "\n", s);
		ft_putstr_fd(s, 1);
		free(s);
		return (0);
	}
	else
	{
		perror("pwd");
		//  1 ou errno ?
		return (1);
	}
	return (0);
}

/* int builtin_env(t_env **env) */
/* { */
/* 	char *s; */
/* 	int i; */
/**/
/* 	i = 0; */
/* 	s = NULL; */
/* 	while (*env) */
/* 	{ */
/* 		s = ft_join(s, *env, s);	 */
/* 		*env = (*env)->next; */
/* 	} */
/* 	ft_putstr_fd(s, 1); */
/* 	return (0); */
/* } */

/* int builtin_unset(t_env **env, char *var) */
/* { */
/* 		//on veut refaire la liste chainee de l'env en virant juste var si on la trouve */
/* 	while (*env) */
/* 	{ */
/* 		if (ft_strncmp(var, (*env)->s, ft_strlen((*env)->s))) */
/* 			*env = (*env)->next; */
/* 		else  */
/* 			//delete le maillon et rejoin la liste */
/* 	} */
/* 	return (0); */
/* } */

/* int builtin_export(t_env **env, char *var) */
/* { */
/* 	while(*env->next) */
/* 		*env = (*env)->next; */
/* 	//ajouter un maillon a env, avec la string var */
/* 	return (0); */
/* } */

//a voir
int builtin_exit(char **arg)
{
	int n;

	if (!arg[1])
		exit (0);
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		exit (1);
	}
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	exit(n);
	return (0);
}

int builtin_cd(char *path)
{
	char *new_path;

	new_path = NULL;
	if (!path)
	{
		//si HOME existe : on chdir(HOME)
		//si non : 
		ft_putstr_fd("minishell: cd: HOME not set", 1);
		return (1);
	}
	//savoir si on a un chemin absolu ou relatif
	//si absolu : 
	chdir(path);
	//si relatif :
	//construire le chemin
	chdir(new_path);
	free(new_path);
	return (0);
}

int	exec(char **arg)
{
	 /* faire un switch case et y mettre les returns */
	if (!ft_strncmp(arg[0], "echo", 4))
		builtin_echo(arg);
	/* else if (!ft_strncmp(arg[0], "cd", 2))  */
	/* 	builtin_cd(arg); */
	else if (!ft_strncmp(arg[0], "pwd", 3))
		builtin_pwd();
	/* else if (!ft_strncmp(arg[0], "export", 6)) */
	/* 	builtin_export(arg); */
	/* else if (!ft_strncmp(arg[0], "unset", 5)) */
	/* 	builtin_unset(arg); */
	/* else if (!ft_strncmp(arg[0], "env", 3)) */
	/* 	builtin_env(arg); */
	else if (!ft_strncmp(arg[0], "exit", 4))
		builtin_exit(arg);
	else // pas printf ! utiliser fd_putstr_fd
		printf("error : not a builtin\n");
	/* return(fct) pour faire remonter le code d'erreur ? */
	return (0);
}
