/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:36:27 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:36:28 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/* DECIDER : */
// - CDPATH
// - cd -
// - cd ~
// - update PWD et OLDPWD

// env -i bash
//
// env
// cd ..
// env
//==> OLDPWD existe et est a jour
// env
// unset PWD
// env
//==>Ne doit afficher ni PWD ni OLD_PWD
// env
// unset PWD
// export PWD=lol
// pwd
// env
//==>ne change PAS la variable PWD
// cd ..
// env
//==>PWD est a jour et OLD_PWD = lo,
//
//
// Quand on fait cd :
// Si la variable n'existe pas : on annule la mise a jour
// si elle existe, on la met a jour, et on update / cree OLD_PWD
// Si PWD est declaree en locale mais pas exportee : on cree OLDPWD a partir de la varialbe localem mais on ne l'exporte pas PWD
// Dans tous les cas,
	/* un echo $PWD doit afficher le path actuel meme si pas exportee */

// ajouter l'acces a des dossiers non permis ?
// cd dans un fichier ?
// mkdir -p a/b/c && cd $_
/* If the directory change is successful,
	cd sets the value of the PWD environment */
/* variable to the new directory name,
	and sets the OLDPWD environment variable */
/* to the value of the current working directory before the change. */
/* The return status is zero if the directory is successfully changed,
	non-zero oth- */
/* erwise */

// gerer CDPATH ?
///* If a non-empty directory name from CDPATH is used,
/* 	or if ‘-’ is the first argu- */
/* ment, and the directory change is successful,
	the absolute pathname of the new */
/* working directory is written to the standard output. */
/* Change the current working directory to directory.
 * If directory is not supplied, the value of the HOME shell variable is used.
 * If the shell variable CDPATH exists,
	it is used as a search path: each directory name in CDPATH is searched for */
/* directory, with alternative directory names in CDPATH separated by a colon */
/* (‘:’).
 * If directory begins with a slash, CDPATH is not used. */
/* By default, or when the -L option is supplied,
	symbolic links in directory are */
/* resolved after cd processes an instance of ‘..’ in directory. */
/* If ‘..’ appears in directory,
	it is processed by removing the immediately pre- */
/* ceding pathname component, back to a slash or the beginning of directory. */
/* If directory is ‘-’,
	it is converted to $OLDPWD before the directory change is */
/* attempted. */
/* If a non-empty directory name from CDPATH is used,
	or if ‘-’ is the first argu- */
/* ment, and the directory change is successful,
	the absolute pathname of the new */
/* working directory is written to the standard output. */

int	change_directory(char *path)
{
	if (!path)
		return (0);
	if (chdir(path) == 0)
		return (0);
	else
	{
		perror(path);
		return (1);
	}
}

int	builtin_cd_without_arg(t_var **env)
{
	char	*s;

	s = get_value(env, "HOME"); // a revoir ?
	if (!s)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 1);
		return (1);
	}
	else
	{
		if (!change_directory(s))
			update_env(env);
	}
	return (0);
}

int	builtin_cd(char **arg, t_var **env)
{
	char	*path;

	/* print_var(env); */
	path = NULL;
	if (array_size(arg) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (array_size(arg) == 1)
		builtin_cd_without_arg(env);
	if (array_size(arg) == 2)
	{
		if (!ft_strncmp(arg[1], "-", 2))
			path = ft_strdup(get_value(env, "OLDPWD"));
		else
			path = ft_strdup(arg[1]);
		if (!change_directory(path))
			update_env(env);
		else
		{
			if (path)
				free(path);
			return (1);
		}
		if (!ft_strncmp(arg[1], "-", 2))
			builtin_pwd();
		if (path && *path)
			free(path);
	}
	return (0);
}
