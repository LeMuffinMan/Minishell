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

// ne pas afficher les variables non exportees ? ou justement on affiche tout ?
// NE PAS AFFICHER LES VARIABLES SANS VALEUR !
// //revoir la valeur de return en cas d'erreur
int	builtin_env(t_var **env)
{
	t_var	*tmp;

	if (!*env)
		return (0);
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
