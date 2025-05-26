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
#include "utils.h"
#include "malloc_error_handlers.h"
#include "structs.h"
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

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

int	update_env(t_var **env)
{
	t_var	*old_pwd;
	t_var	*pwd;
	char	buf[PATH_MAX];

	errno = 0;
	(void)buf;
	old_pwd = is_known_key(env, "OLDPWD");
	if (!old_pwd)
	{
		add_back_var(env, "OLDPWD=", 3);
		old_pwd = is_known_key(env, "OLDPWD");
	}
	pwd = is_known_key(env, "PWD");
	if (!pwd)
	{
		add_back_var(env, "PWD=", 3);
		pwd = is_known_key(env, "PWD");
	}
	if (old_pwd->value)
		free(old_pwd->value);
	if (pwd->value)
		old_pwd->value = ft_strdup(pwd->value);
	else
		old_pwd->value = ft_strdup("");
	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		if (pwd->value)
			free(pwd->value);
		pwd->value = ft_strdup(buf);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
		pwd->value = ft_strjoin(pwd->value, "/..");
		//on check errno a la sortie pour un potnetiel malloc qui a foire !
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


int	append_to_var(t_var **node, char **key_value)
{
	char *s;
	char *tmp;
	int saved_errno;

	s = ft_strdup((*node)->value);
	if (!s)
		return (errno);
	tmp = s;
	s = ft_strjoin(s, key_value[1]);
	if (!s)
	{
		saved_errno = errno;
		free(tmp);
		errno = saved_errno;
		return (errno);
	}
	free(tmp);
	free((*node)->value);	
	(*node)->value = s;
	return (0);
}

int	update_var(t_var **node, char **key_value, int inc)
{
	int	err;

	if (inc == 1)
	{
		err = append_to_var(node, key_value);
		if (err)
			return (err);
	}
	else
	{
		free((*node)->value);
		if (!key_value[1])
		{
			key_value[1] = ft_strdup("");
			if (!key_value[1])
				return (errno);
		}
		(*node)->value = ft_strdup(key_value[1]);
		if (!(*node)->value)
			return (errno);
	}
	(*node)->exported = 1;
	return (0);
}


int update_existing_value(t_var *node, char *var, int inc)
{
	char *value;
	char *tmp;

	value = ft_strchr(var, '=');
	if (!value)
		return (0);
	if (inc)
	{
		tmp = node->value;
		node->value = ft_strjoin(node->value, value + 1);
		if (!node->value)
			return (malloc_free_string(tmp));
		free(tmp);
	}
	else
	{
		free(node->value);
		node->value = ft_strdup(value + 1);
		if (!node->value)
			return (errno);
	}
	return (0);
}

t_var *update_if_known_key(t_var **env, char *var, char *s, int inc)
{
	t_var *node;
	char **key;

	key = ft_split(s, '=');
	if (!key)
		return (NULL);
	node = is_known_exported_key(env, key[0]);
	free_array(key);
	if (node)
	{
		if (update_existing_value(node, var, inc) == -1)
			return (NULL);
	}
	return node;
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

char	*copy_after_plus(char *arg, char *s, int i)
{
	int j;

	s[i] = '=';
	j = i + 1;
	i += 2;
	while (arg[i])
	{
		s[j] = arg[i];
		j++;
		i++;
	}
	s[j] = '\0';
	return (s);
}


char	*clean_inc_operator(char *arg)
{
	int i;
	char *s;

	i = 0;
	if (!arg)
		return (NULL);
	s = malloc(sizeof(char) * (ft_strlen(arg) + 1));
	if (!s)
		return (NULL);
	while (arg[i] && arg[i] != '+')
	{
		s[i] = arg[i];
		i++;
	}
	if (arg[i] != '+')
	{
		s[i] = '\0';
		return (s);
	}
	return (copy_after_plus(arg, s, i));
}



char *prepare_var_string(char *var, int *inc)
{
	char *s;

	if (is_an_incrementation(var))
	{
		*inc = 1;
		s = clean_inc_operator(var);
		if (!s)
			return (NULL);
	}
	else
	{
		s = ft_strdup(var);
		if (!s)
			return (NULL);
	}
	return (s);
}

int add_or_update_var(t_var **env, char *var)
{
	int inc;
	char *s;
	t_var *node;

	inc = 0;
	s = prepare_var_string(var, &inc);
	if (!s)
		return (errno);
	node = update_if_known_key(env, var, s, inc);
	if (errno == ENOMEM)
		return (malloc_free_string(s));
	if (!node)
	{
		add_back_var(env, s, 3);
		if (errno == ENOMEM)
			return (malloc_free_string(s));
	}
	free(s);
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

