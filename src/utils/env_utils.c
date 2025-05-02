/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:15:02 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:28:49 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "structs.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

t_var	*get_key_node(t_var **env, char *key)
{
	t_var	*tmp;

	tmp = *env;
	if (!key)
		return (NULL);
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, key, (ft_strlen(key) + 1)))
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

//  return (1) ou errno ?
int	update_env(t_var **env)
{
	t_var	*old_pwd;
	t_var	*pwd;
	char	buf[PATH_MAX];

	(void)buf;
	old_pwd = get_key_node(env, "OLDPWD");
	if (!old_pwd)
	{
		//ajouter le old_pwd a l'env
	}
	pwd = get_key_node(env, "PWD");
	if (old_pwd->value)
		free(old_pwd->value);
	old_pwd->value = ft_strdup(pwd->value);
	if (pwd->value)
		free(pwd->value);
	if (getcwd(buf, sizeof(buf)) != NULL)
		pwd->value = ft_strdup(buf);
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}

// recupere une valeur dans l'environnement par sa cle (export, env ET local)
char	*get_value(t_var **env, char *key)
{
	char	*value;
	t_var	*tmp;

	if (!env || !*env)
		return (NULL);
	tmp = *env;
	value = NULL;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(key)))
		{
			value = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	return (value);
}

// et si on me declare des varialbes _=_ ?
//
//
// Attention :
// echo $_ renvoie le dernier argument : echo; echo $_ : echo
// echo hello; echo $_ : hello
// !tmp nedevrait pas etre possible
// lastcmd doit contenir le PATH !
int	update_last_cmd_var(t_var **env, char *last_cmd)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp("_", tmp->key, 2))
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
		add_back_var(env, last_cmd, 1);
	else
	{
		printf("update\n");
		free(tmp->value);
		tmp->value = ft_strdup(last_cmd);
	}
	return (0);
}
