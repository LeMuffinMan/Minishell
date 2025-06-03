/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:03:53 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 19:08:34 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

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

static void	add_space_for_wildcard(t_token **node)
{
	int		i;
	char	*res;

	i = 0;
	while ((*node)->content[i + 1])
	{
		res = ft_strjoin((*node)->content[i], " ");
		if (!res)
		{
			free_parse(*node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
			return ;
		}
		free((*node)->content[i]);
		(*node)->content[i] = ft_strdup(res);
		free(res);
		if (!(*node)->content[i])
		{
			free_parse(*node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
			return ;
		}
		i++;
	}
}

static bool	make_wildcard(t_token **node, bool flag)
{
	char	**current_dir;
	int		sort_of;

	current_dir = extract_current_dir();
	if (!current_dir || !*current_dir)
		return (false);
	sort_of = which_case_of_wildcard((*node)->content[0]);
	if (sort_of == INFIX)
		is_infix(node, current_dir);
	else if (sort_of == PREFIX)
		is_prefix(node, current_dir);
	else if (sort_of == SUFFIX)
		is_suffix(node, current_dir);
	else
		is_joker(node, current_dir);
	free_tab(current_dir);
	if (errno == MEM_ALLOC)
		return (false);
	if (flag)
		add_space_for_wildcard(node);
	if (errno == MEM_ALLOC)
		return (false);
	return (true);
}

bool	handle_wildcard(t_token **head, bool flag)
{
	t_token	*tmp;
	bool	is_echo;

	if (!head || !*head || !flag)
		return (true);
	tmp = *head;
	is_echo = false;
	while (tmp)
	{
		if (tmp->token == WILDCARD || is_wildcard(tmp->content[0]))
		{
			if (tmp->prev
				&& !ft_strncmp(tmp->prev->content[0], "echo", 5))
				is_echo = true;
			if (!make_wildcard(&tmp, is_echo))
			{
				free_parse(*head, NULL, MEM_ALLOC);
				errno = MEM_ALLOC;
				return (false);
			}
		}
		tmp = tmp->next;
	}
	return (true);
}
