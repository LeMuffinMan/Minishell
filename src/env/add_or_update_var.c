/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_or_update_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:37:22 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:37:24 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>

static int	is_an_incrementation(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=' && arg[i - 1] == '+')
			return (1);
		i++;
	}
	return (0);
}

static char	*copy_after_plus(char *arg, char *s, int i)
{
	int	j;

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

static char	*clean_inc_operator(char *arg)
{
	int		i;
	char	*s;

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

static char	*prepare_var_string(char *var, int *inc)
{
	char	*s;

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

int	add_or_update_var(t_var **env, char *var)
{
	int		inc;
	char	*s;
	t_var	*node;

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
