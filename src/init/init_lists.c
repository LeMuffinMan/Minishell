/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:12:30 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 16:13:15 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdlib.h>

// TODO
// Mettre des calloc partout
// faire un fichier ultrabonus_init_lists.c

// METTRE QUE DES CALLOC !
int	init_lists_env(t_lists *lists)
{
	lists->env = NULL;
	lists->env = malloc(sizeof(t_var *));
	if (!lists->env)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->env = NULL;
	return (0);
}

int	init_lists_ast(t_lists *lists)
{
	lists->ast = NULL;
	lists->ast = malloc(sizeof(t_tree *));
	if (!lists->ast)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->ast = NULL;
	return (0);
}

int	init_lists_pipes(t_lists *lists)
{
	lists->pipes = NULL;
	lists->pipes = malloc(sizeof(t_pipe *));
	if (!lists->pipes)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->pipes = NULL;
	return (0);
}

int init_lists_origin_fds(t_lists *lists)
{
    lists->origin_fds[0] = -1;
    lists->origin_fds[1] = -1;
    return (0);
}

// Ultra bonus
//
int	init_lists_history(t_lists *lists)
{
	lists->history = NULL;
	lists->history = malloc(sizeof(t_hist *));
	if (!lists->history)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->history = NULL;
	return (0);
}

int	init_lists_alias(t_lists *lists)
{
	lists->aliases = NULL;
	lists->aliases = malloc(sizeof(t_alias *));
	if (!lists->aliases)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->aliases = NULL;
	return (0);
}

int	init_lists_shell_functions(t_lists *lists)
{
	lists->shell_fcts = NULL;
	lists->shell_fcts = malloc(sizeof(t_shell_fct *));
	if (!lists->shell_fcts)
	{
		free_lists(lists);
		return (-1);
	}
	*lists->shell_fcts = NULL;
	return (0);
}
// ULtra bonus
//

int	init_lists(t_lists *lists)
{
	/* lists = malloc(sizeof(t_lists)); */
	if (!lists)
		return (-1);
	if (init_lists_env(lists) == -1)
		return (-1);
	if (init_lists_ast(lists) == -1)
		return (-1);
	if (init_lists_pipes(lists) == -1)
		return (-1);
	if (init_lists_origin_fds(lists) == -1)
		return (-1);
	if (init_lists_history(lists) == -1)
		return (-1);
	if (init_lists_alias(lists) == -1)
		return (-1);
	if (init_lists_shell_functions(lists) == -1)
		return (-1);
	lists->exit_code = 0;
	return (0);
}
