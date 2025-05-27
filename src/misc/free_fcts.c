/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fcts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:40:14 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:40:15 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "structs.h"
#include <stdlib.h>

int	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}

int	free_aliases(t_alias **aliases)
{
	t_alias	*tmp;
	t_alias	*next;

	// ULTRABONUS !!!
	if (!aliases || !*aliases)
		return (0);
	tmp = *aliases;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->name);
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
	*aliases = NULL;
	return (0);
}

int	free_shell_fcts(t_shell_fct **shell_fcts)
{
	t_shell_fct	*tmp;
	t_shell_fct	*last;

	// ULTRABONUS !!!
	if (!shell_fcts || !*shell_fcts)
		return (0);
	tmp = *shell_fcts;
	last = NULL;
	while (tmp)
	{
		if (last)
		{
			free(last->name);
			free_array(last->content);
			free(last);
		}
		last = tmp;
		tmp = tmp->next;
	}
	if (last)
	{
		free(last->name);
		free_array(last->content);
		free(last);
	}
	*shell_fcts = NULL;
	return (0);
}

void	free_lists(t_lists *lists)
{
	t_tree	*tree_to_free;

	if (!lists)
		return ;
	if (lists->env)
	{
		free_list(lists->env);
		free(lists->env);
	}
	// ULTRABONUS
	if (lists->history)
	{
		free_history(lists->history);
		free(lists->history);
	}
	if (lists->ast)
	{
		if (*lists->ast)
		{
			tree_to_free = *lists->ast;
			free_tree(&tree_to_free);
		}
		free(lists->ast);
	}
	if (lists->pipes)
		free(lists->pipes);
	if (lists->aliases)
	{
		if (*lists->aliases)
			free_aliases(lists->aliases);
		free(lists->aliases);
	}
	if (lists->shell_fcts)
	{
		if (*lists->shell_fcts)
			free_shell_fcts(lists->shell_fcts);
		free(lists->shell_fcts);
	}
}
