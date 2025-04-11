/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:12:20 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:46:23 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int	print_array(char **array)
{
	int		i;
	char	*output;

	i = 0;
	while (array[i])
	{
		output = ft_strjoin(output, array[i], output);
		output = ft_strjoin(output, "\n", output);
	}
	ft_putstr_fd(output, 1);
	free(output);
	return (0);
}

int	print_sorted_env(t_var **env)
{
	t_var	*tmp;
	char	*s;

	tmp = *env;
	s = ft_strdup("declare -x ");
	while (tmp)
	{
		s = ft_strjoin(s, tmp->key, s);
		if (tmp->value)
		{
			s = ft_strjoin(s, "=\"", s);
			s = ft_strjoin(s, tmp->value, s);
			s = ft_strjoin(s, "\"\n", s);
		}
		ft_putstr_fd(s, 1);
		free(s);
		tmp = tmp->next;
	}
	return (0);
}

#include <stdio.h> // a virer et utilisetr ft_putstr_fd
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
