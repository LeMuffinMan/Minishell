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

#include "libft.h"
#include "export_utils.h"
#include "env_utils.h"
#include "malloc_error_handlers.h"
#include <errno.h>

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
