/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:12:20 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:04:22 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include "libft.h"
#include "structs.h"
#include <errno.h>
#include <stdio.h>

int	print_sorted_env(t_var **env)
{
	t_var	*tmp;
	int		exit_code;

	tmp = *env;
	while (tmp)
	{
		if (tmp->exported == 1)
		{
			exit_code = print_export_line(tmp);
			if (exit_code != 0)
				return (exit_code);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	print_var(t_var **env)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->key && tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		if (tmp->key && !tmp->value)
			printf("%s\n", tmp->key);
		tmp = tmp->next;
	}
	return (0);
}

int	print_env(t_var **env)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->env == 1)
		{
			if (tmp->key)
				ft_putstr_fd(tmp->key, 1);
			if (tmp->value)
			{
				ft_putstr_fd("=", 1);
				ft_putstr_fd(tmp->value, 1);
			}
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
