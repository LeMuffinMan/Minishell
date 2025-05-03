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

#include "exec.h"
#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>

int	print_array(char **array)
{
	int		i;
	char	*output;

	i = 0;
	while (array[i])
	{
		output = ft_strjoin(array[i], "\n");
		ft_putstr_fd(output, 1);
		free(output);
		i++;
	}
	return (0);
}

int	print_sorted_env(t_var **env)
{
	t_var	*tmp;
	char	*s;
	char	*temp;

	tmp = *env;
	while (tmp)
	{
		/* printf("key = %s | exported = %d\n", tmp->key, tmp->exported); */
		//cette condition casse tout !
		/* if (tmp->exported == 1) */
		/* { */
			s = ft_strjoin("declare -x ", tmp->key);
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
		/* } */
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
