/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:58:12 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:40:50 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "structs.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int	node_to_str(t_var *tmp, char **array, int i)
{
	char	*s;
	char	*temp;

	if (tmp->value)
	{
		s = ft_strjoin(tmp->key, "=");
		if (errno == ENOMEM)
			return (errno);
		temp = s;
		array[i] = ft_strjoin(s, tmp->value);
		free(temp);
	}
	else
		array[i] = ft_strdup(tmp->key);
	if (!array[i])
	{
		while (i-- > 0)
			free(array[i]);
		free(array);
		return (ENOMEM);
	}
	return (0);
}

char	**lst_to_array(t_var **env)
{
	t_var	*tmp;
	char	**array;
	int		i;
	int		count;

	count = 0;
	tmp = *env;
	while (tmp && ++count)
		tmp = tmp->next;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	tmp = *env;
	i = 0;
	while (tmp)
	{
		node_to_str(tmp, array, i);
		if (errno == ENOMEM)
			return (NULL);
		i++;
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}

int	array_size(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

int	is_interactive_mode(void)
{
	if (!isatty(0))
	{
		ft_putstr_fd("Minishell does not support non-interactive mode\n", 2);
		exit(0);
	}
	return (0);
}
