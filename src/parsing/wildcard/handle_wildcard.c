/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:03:53 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 20:58:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

bool	is_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

bool	handle_current_dir(int *i, char ***res)
{
	DIR	*dir;
	struct dirent *current;

	errno = 0;
	dir = opendir(".");
	if (!dir)
		return (false);
	current = readdir(dir);
	while (current)
	{
		if (ft_strncmp(current->d_name, ".", 2) && ft_strncmp(current->d_name, "..", 3))
		{
			if (!res)
				(*i)++;
			else
			{
				*res[*i] = ft_strdup(current->d_name);
				if (!*res)
					return (false);
			}
		}
	}
	if (errno)
		return (false);
	closedir(dir);
}

static char	**extract_current_dir(void)
{
	int	i;

	i = 0;
	
}

static bool	make_wildcar(t_token **node)
{
	char	**current_dir;

	current_dir = extract_current_dir();
	if (!current_dir)
		return (false);
	return (true);
}

void	handle_wildcard(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == WILDCARD)
		{
			if (!make_wildcard(&tmp))
				free_parse(*head, "Malloc failed in function 'make_wildcard'", MEM_ALLOC);
		}
		tmp = tmp->next;
	}
}
