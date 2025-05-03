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
#include <unistd.h>

int update_underscore_var(t_var **env)
{
	t_var *tmp;

	tmp = *env;
	while(tmp)
	{
		if (!ft_strncmp(tmp->key, "_", 2))
		{
			free(tmp->value);
			tmp->value = ft_strdup("env");
		}
		tmp = tmp->next;
	}
	return (0);
}


// ne pas afficher les variables non exportees ? ou justement on affiche tout ?
// NE PAS AFFICHER LES VARIABLES SANS VALEUR !
// //revoir la valeur de return en cas d'erreur
int	builtin_env(t_var **env, char **content)
{
	t_var	*tmp;
	char *s;
	char *temp;

	if (!*env)
		return (0);
	if (content[1])
	{
		s = ft_strjoin("env: \'", content[1]);
		temp = s;
		s = ft_strjoin(s, "\': No option or argument accepted for env builtin\n");
		free(temp);
		ft_putstr_fd(s, 2);
		free(s);
		return (127);
	}
	update_underscore_var(env);
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
