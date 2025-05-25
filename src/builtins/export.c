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
#include "malloc_error_handlers.h"
#include <errno.h>

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

char *get_full_variable_declaration(char **arg, int i, char **s)
{
	if (arg[i][ft_strlen(arg[i]) - 1] == '=') 
	{
		if (arg[i + 1] && !is_var_declaration(arg[i + 1])) 
		{
			*s = ft_strjoin(arg[i], arg[i + 1]);
			if (!*s)
				return (NULL);
		}
		else
		{
			*s = ft_strdup(arg[i]); 
			if (!*s)
				return (NULL);
		}
	}
	else 
	{
		*s = ft_strdup(arg[i]);
		if (!*s)
			return (NULL);
	}
	return (*s);
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

int builtin_export_var_declaration(t_var **env, char **arg, int i, char **s)
{
	int saved_errno;

	get_full_variable_declaration(arg, i, s);
	if (errno == ENOMEM)
		return (errno);
	add_or_update_var(env, *s);
	if (errno == ENOMEM)
	{
		saved_errno = errno;
		free(*s);
		errno = saved_errno;
		return (errno);
	}
	free(*s);
	return (0);
} 

int builtin_export_var_exportation(t_var **env, char *arg)
{
	t_var *node;
	node = is_known_key(env, arg);
	if (node)
		node->exported = 1;
	else 
	{
		add_or_update_var(env, arg);
		if (errno == ENOMEM)
			return (errno);
	}
	return (0);
}

int	parse_export_arguments(t_var **env, char **arg, int i, int *exit_code)
{
	char	*s;

	if (is_var_declaration(arg[i]))
	{
		if (builtin_export_var_declaration(env, arg, i, &s) != 0)
			return (errno);
	}
	else if (is_var_exportation(arg[i]))
	{
		if (builtin_export_var_exportation(env, arg[i]) != 0)
			return (errno);
	}
	else
	{
		*exit_code = 1;
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
	}
	return (0);
}

int	process_export_args(t_var **env, char **arg)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	while (arg[i])
	{
		if (parse_export_arguments(env, arg, i, &exit_code) != 0)
			return (errno);
		i++;
	}
	return (exit_code);
}

int	builtin_export(t_var **env, char **arg)
{
	if (!arg[1])
		return (export_without_argument(env));
	return (process_export_args(env, arg));
}
