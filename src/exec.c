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
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
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

// tests :
// echo
// echo -n
// echo -n -n -n
// echo -n hello
// echo -n -n hello
// echo hello
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
		return (0);
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
		while (arg[i] && !ft_strncmp(arg[i], "-n", 2) && ft_strlen(arg[1]) == 2)
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
	if (s)
		ft_putstr_fd(s, 1);
	free(s);
	return (0);
}

// tester en unsetant PWD ...
int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	char	*s;

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



// a voir
int	builtin_exit(char **arg, t_env **env)
{
	int	n;

	if (!arg[1])
	{
		free_list(env); //double free ?
		exit(0);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		exit(1);
	}
	//Si on a autre chose que des digits : 
	//message d'erreur + retour d'erreur 2
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	exit(n);
	return (0);
}

//ajouter l'acces a des dossiers non permis ?
int	builtin_cd(char **arg)
{
	char	*s;

	/* char *new_path; */
	/* char	buf[PATH_MAX]; */
	/* new_path = NULL; */
	s = NULL;
	if (!arg[1])
	{
		// si HOME existe : on chdir(HOME)
		// si non :
		ft_putstr_fd("minishell: cd: HOME not set", 1);
		return (1);
	}
	if (arg[1][0] && arg[1][0] == '/') // Si on chere cd ~ on doit adapter ca
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
		else
		{
			s = ft_join("minishell: cd: ", arg[1], s);
			s = ft_join(s, ": No such file or directory\n", s);
			ft_putstr_fd(s, 2);
			free(s);
			return (1);
		}
	}
	else if (arg[1][0]) // on est sur un chemin relatif 
	{
		if (access(arg[1], F_OK) == 0)
			chdir(arg[1]); // a proteger !
	}
	/* free(new_path); */
	return (0);
}

int builtin_env(t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	return (0);
}

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
/**/
int builtin_export(t_env **env, char **arg)
{
	t_env *tmp;
	char *s;
	int i;
	char **line;
	
	i = 0;
	s = NULL;
	line = NULL;
	tmp = *env;
	if (!arg[1]) //export sans arguments affiche l'env avec declare + x au debut de chaque lignes 
	{
		while (tmp)
		{
			//le format d'affichage est pas bon : VAR="ewfwe"
			s = ft_join(s, "declare -x ", s);
			line = ft_split(tmp->line, '=');
			s = ft_join(s, line[0], s);
			s = ft_join(s, "=\"", s);
			if (line[1])
				s = ft_join(s, line[1], s);
			s = ft_join(s, "\"\n", s);
			ft_putstr_fd(s, 1);
			tmp = tmp->next;
			free(s);
			s = NULL;
			ft_free(line);
			line = NULL;
		}
		if (s)
			free(s);
		if (line)
			ft_free(line);
		return (0);
	}
	else //Il faut gerer export VAR=efef VAR1=fewfwe VAR2=efgeg
	{
		while (arg[i])
		{

			i++;
		}
	}
	while(tmp->next)
		tmp = tmp->next;
	//ajouter un maillon a env, avec la string var
	return (0);
}

int	exec(char **arg, t_env **env)
{
	/* faire un switch case et y mettre les returns */
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 4))
		return(builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 2))
		return(builtin_cd(arg));
	else if (!ft_strncmp(arg[0], "pwd", 3))
		return(builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 6))
		return (builtin_export(env, arg));
	/* else if (!ft_strncmp(arg[0], "unset", 5)) */
	/* 	builtin_unset(arg); */
	else if (!ft_strncmp(arg[0], "env", 3))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 4))
		return(builtin_exit(arg, env));
	else // pas printf ! utiliser fd_putstr_fd
		printf("error : not a builtin\n");
	return (0);
}
