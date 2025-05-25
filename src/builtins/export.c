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
#include <errno.h>

int update_var(t_var **node, char **key_value, int inc)
{
	char *s;
	char *tmp;
	int saved_errno;

	if (inc == 1)
	{
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
	return(0);
}

int export_without_argument(t_var **env)
{
	t_var *copy;

	copy = NULL;
	copy = copy_list(env);
	if (!copy)
		return (errno);
	sort_list(&copy);
	if (errno == ENOMEM)
		return (errno);
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
		if (arg[i] == '+' && arg[i + 1] && arg[i + 1] != '=') // += pas valide !
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

	s = malloc(sizeof(char) * (ft_strlen(arg)));
	if (!s)
		return (NULL);
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
	return (s);
}


char *get_full_variable_declaration(char **arg, int i)
{
	char *s;

	if (arg[i][ft_strlen(arg[i]) - 1] == '=') //si s finit par = 
	{
		if (arg[i + 1] && !is_var_declaration(arg[i + 1])) //et que content[i + 1] est sa valeur
		{
			s = ft_strjoin(arg[i], arg[i + 1]);
			if (!s)
				return (NULL);
		}
		else
		{
			s = ft_strdup(arg[i]); //si on a une declaration de var en content[i + 1]
			if (!s)
				return (NULL);
		}
	}
	else //si on a un cas classique
	{
		s = ft_strdup(arg[i]);
		if (!s)
			return (NULL);
	}
	return (s);
}

int add_or_update_var(t_var **env, char *var)
{
	int inc;
	char *s;
	char *tmp;
	char *value;
	char **key;
	int saved_errno;
	t_var *node;

	inc = 0;
	if (is_an_incrementation(var))
	{
		s = clean_inc_operator(var);
		if (errno == ENOMEM)
			return (errno);
		inc = 1;
	}
	else
	{
		s = ft_strdup(var);
		if (!s)
			return (errno);
	}
	key = ft_split(s, '='); //surement qu'on peut faire plus simple au'un split
	if (!key)
	{
		saved_errno = errno;
		free(s);
		errno = saved_errno;
		return (errno);
	}
	node = is_known_exported_key(env, key[0]);
	free_array(key);
	if (node) // si on l'a deja
	{
		value = ft_strchr(var, '=');
		if (inc)
		{
			tmp = node->value;
			node->value = ft_strjoin(node->value, value + 1);
			if (!node->value)
			{
				saved_errno = errno;
				free(s);
				errno = saved_errno;
				return (errno);
			}
			free(tmp);
		}
		else
		{
			free(node->value);
			node->value = ft_strdup(value + 1);
			if (!node->value)
			{
				saved_errno = errno;
				free(s);
				errno = saved_errno;
				return (errno);
			}
		}
	}
	else 
	{
		add_back_var(env, s, 3);
		if (errno == ENOMEM)
			return (errno);
	}
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
		if ((s[i] < '0' || s[i] > 'z'))
			return (0);
		i++;
	}
	if (s[i] == '=')
		return (0);
	return (1);
}

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
			if (errno == ENOMEM)
				return (errno);
			add_or_update_var(env, s);
			if (errno == ENOMEM)
			{
				free(s);
				return (errno);
			}
			free(s);
		}
		else if (is_var_exportation(arg[i]))
		{

			node = is_known_key(env, arg[i]);
			if (node)
				node->exported = 1;
			else 
			{
				add_or_update_var(env, arg[i]);
				if (errno == ENOMEM)
					return (errno);
			}
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
