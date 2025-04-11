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
		output = ft_strjoin(array[i], "\n");
	ft_putstr_fd(output, 1);
	free(output);
	return (0);
}

int	print_sorted_env(t_var **env)
{
	t_var	*tmp;
	char	*s;
	char *temp;

	tmp = *env;
	s = ft_strdup("declare -x ");
	while (tmp)
	{
		s = ft_strjoin(s, tmp->key);
		temp = s;
		if (tmp->value)
		{
			s = ft_strjoin(s, "=\"");
			free(temp);
			temp = s;
			s = ft_strjoin(s, tmp->value);
			free(temp);
			temp = s;
			s = ft_strjoin(s, "\"\n");
			free(temp);
			temp = s;
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
