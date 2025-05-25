/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:31:29 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:31:43 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int update_underscore_var(t_var **env)
{
	t_var *tmp;

	tmp = *env;
	while(tmp)
	{
		if (!ft_strncmp(tmp->key, "_", 2) && tmp->env == 1)
		{
			free(tmp->value);
			tmp->value = ft_strdup("env");
			if (!tmp->value)
				return (errno);
		}
		tmp = tmp->next;
	}
	return (0);
}

int error_env(char **content)
{
	char *s;
	char *temp;
	int saved_errno;

	s = ft_strjoin("env: \'", content[1]);
	if (!s)
		return (errno);
	temp = s;
	s = ft_strjoin(s, "\': No option or argument accepted for env builtin\n");
	if (!s)
	{
		saved_errno = errno;
		free(temp);
		errno = saved_errno;
		return (errno);
	}
	free(temp);
	ft_putstr_fd(s, 2);
	free(s);
	return (127);
}

// ne pas afficher les variables non exportees ? ou justement on affiche tout ?
// NE PAS AFFICHER LES VARIABLES SANS VALEUR !
// //revoir la valeur de return en cas d'erreur
int	builtin_env(t_var **env, char **content)
{
	t_var	*tmp;

	if (!*env)
		return (0);
	if (content[1])
		return(error_env(content));
	update_underscore_var(env);
	if (errno == ENOMEM)
		return (errno);
	tmp = *env;
	while (tmp)
	{
		if (tmp->value && tmp->env == 1)
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	return (0);
}
