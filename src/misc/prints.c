/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:12:20 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:31:03 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <stdio.h>
#include <errno.h>

static char	*create_base_export_line(t_var *var)
{
	char	*line;

	line = ft_strjoin("declare -x ", var->key);
	if (errno == ENOMEM)
		return (NULL);
	return (line);
}

static char *join_with(char *line, char *to_join)
{
	char *new_line;

	new_line = ft_strjoin(line, to_join);
  if (errno == ENOMEM)
  {
      free(line);
      return (NULL);
  }
  free(line);
  return (new_line);
}

static char *append_value_to_line(char *line, t_var *var)
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

static char	*create_export_line(t_var *var)
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

static int	print_export_line(t_var *var)
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

int print_env(t_var **env)
{
    t_var *tmp;

    tmp = *env;
    while (tmp)
    {
    	if (tmp->env == 1)
    	{
        if (tmp->key)
            ft_putstr_fd(tmp->key, 1);
        if (tmp->value) //Si on fait VAR= on ft_strdup("")
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


int	print_all_variables(t_var **env)
{
	t_var	*tmp;

	tmp = *env;
	while (tmp)
	{
		printf("key = %s | value = %s | exported = %d | env = %d\n", tmp->key,
			tmp->value, tmp->exported, tmp->env);
		tmp = tmp->next;
	}
	return (0);
}
