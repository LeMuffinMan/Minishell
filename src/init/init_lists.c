/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:12:30 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:42:12 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include <stdlib.h>

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

int	init_lists_origin_fds(t_lists *lists)
{
	lists->origin_fds[0] = -1;
	lists->origin_fds[1] = -1;
	return (0);
}

int	init_lists(t_lists *lists)
{
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
