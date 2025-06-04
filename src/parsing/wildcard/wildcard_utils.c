/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:29:40 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:17:50 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

bool	handle_current_dir(int *i, char ***res)
{
	DIR				*dir;
	struct dirent	*current;

	dir = opendir(".");
	if (!dir)
		return (false);
	current = readdir(dir);
	while (current)
	{
		if (ft_strncmp(current->d_name, ".", 1))
		{
			if (!res)
				(*i)++;
			else
			{
				(*res)[*i] = ft_strdup(current->d_name);
				if (!(*res)[*i])
					return (false);
				(*i)++;
			}
		}
		current = readdir(dir);
	}
	closedir(dir);
	return (true);
}

void	add_space_for_wildcard(t_token **node)
{
	int		i;
	char	*res;

	i = 0;
	while ((*node)->content[i + 1])
	{
		res = ft_strjoin((*node)->content[i], " ");
		if (!res)
		{
			free_parse(*node);
			errno = MEM_ALLOC;
			return ;
		}
		free((*node)->content[i]);
		(*node)->content[i] = ft_strdup(res);
		free(res);
		if (!(*node)->content[i])
		{
			free_parse(*node);
			errno = MEM_ALLOC;
			return ;
		}
		i++;
	}
}

void	sort_wildcard_arg(char **array)
{
	int		i;
	bool	sorted;
	char	*tmp;

	if (!array)
		return ;
	sorted = false;
	while (!sorted)
	{
		sorted = true;
		i = 0;
		while (array[i] && array[i + 1])
		{
			if (compare_content(array[i], array[i + 1]) > 0)
			{
				tmp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = tmp;
				sorted = false;
			}
			i++;
		}
	}
}

void	verif_wildcard(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->token == WILDCARD)
		{
			if ((!tmp->prev && !tmp->next)
				|| (!tmp->prev && tmp->next
					&& tmp->next->token == PIPE)
				|| (tmp->prev && (tmp->prev->token == PIPE
						|| tmp->prev->token == O_AND
						|| tmp->prev->token == O_OR))
				|| (tmp->next
					&& (tmp->next->error == PERMISSION_DENIED
						|| tmp->next->error == CMD_NOT_FOUND)))
			{
				tmp->token = NO_TOKEN;
				tmp->error = CMD_NOT_FOUND;
			}
		}
		tmp = tmp->next;
	}
}
