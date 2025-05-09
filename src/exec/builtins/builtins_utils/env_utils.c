/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:15:02 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/30 23:13:46 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "structs.h"
#include <limits.h>
#include <stdio.h>
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
		// ajouter le old_pwd a l'env
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
	/* if (tmp) */
	/* 	printf("%p\n", tmp); */
	/* else */
	/* 	printf("waat"); */
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1))
		{
			value = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	/* if (tmp) */
		/* printf("returned node : %p : %s=%s\n", tmp, tmp->key, tmp->value); */
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
		/* printf("update\n"); */
		free(tmp->value);
		tmp->value = ft_strdup(last_cmd);
	}
	return (0);
}

// a tester !
int	update_last_arg_var(t_var **env, char **content)
{
	t_var	*tmp;
	int		i;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "_", 2) && tmp->env == 0)
		{
			i = 0;
			while (content[i])
				i++;
			if (tmp->value) // mettre cette protection partout !
				free(tmp->value);
			tmp->value = ft_strdup(content[i]);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

t_var	*is_known_key(t_var **env, char *key)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	print_all_variables(t_var **env)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		printf("key = %s | value = %s | exported = %d | env = %d\n", tmp->key,
			tmp->value, tmp->exported, tmp->env);
		tmp = tmp->next;
	}
	return (0);
}

int	update_exit_code_var(t_var **env, int exit_code)
{
	t_var	*tmp;

	tmp = is_known_key(env, "?");
	if (!tmp)
		return (1);
	free(tmp->value);
	tmp->value = ft_itoa(exit_code);
	return (0);
}
