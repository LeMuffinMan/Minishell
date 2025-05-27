/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sorted_env_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:01:41 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:03:49 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <errno.h>

char	*create_base_export_line(t_var *var)
{
	char	*line;

	line = ft_strjoin("declare -x ", var->key);
	if (errno == ENOMEM)
		return (NULL);
	return (line);
}

char	*join_with(char *line, char *to_join)
{
	char	*new_line;

	new_line = ft_strjoin(line, to_join);
	if (errno == ENOMEM)
	{
		free(line);
		return (NULL);
	}
	free(line);
	return (new_line);
}

char	*append_value_to_line(char *line, t_var *var)
{
	line = join_with(line, "=\"");
	if (!line)
		return (NULL);
	line = join_with(line, var->value);
	if (!line)
		return (NULL);
	line = join_with(line, "\"");
	if (!line)
		return (NULL);
	return (line);
}

char	*create_export_line(t_var *var)
{
	char	*line;

	line = create_base_export_line(var);
	if (!line)
		return (NULL);
	if (var->value)
	{
		line = append_value_to_line(line, var);
		if (!line)
			return (NULL);
	}
	return (line);
}

int	print_export_line(t_var *var)
{
	char	*line;
	char	*temp;

	line = create_export_line(var);
	if (!line)
		return (errno);
	temp = line;
	line = ft_strjoin(line, "\n");
	if (errno == ENOMEM)
	{
		free(temp);
		return (errno);
	}
	free(temp);
	ft_putstr_fd(line, 1);
	free(line);
	return (0);
}
