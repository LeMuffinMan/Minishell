/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:03:53 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/22 00:32:22 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
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
				(*res)[*i] = ft_strdup(current->d_name);
				if (!(*res)[*i])
					return (false);
				(*i)++;
			}
		}
		current = readdir(dir);
	}
	if (errno)
		return (false);
	closedir(dir);
	return (true);
}

static char	**extract_current_dir(void)
{
	int		i;
	char	**res;

	i = 0;
	handle_current_dir(&i, NULL);
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (!handle_current_dir(&i, &res))
	{
		free_tab(res);
		return (NULL);
	}
	res[i] = NULL;
	return (res);
}

static int	which_case_of_wildcard(char *str)
{
	int	sort_of;
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
		{
			if (i > 0)
			{
				if (str[i + 1])
					sort_of = INFIX;
				else
					sort_of = PREFIX;
			}
			else if (str[i + 1])
				sort_of = SUFFIX;
			else
				sort_of = JOKER;
		}
		i++;
	}
	return (sort_of);
}

static bool	find_prefix(char *str, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[j] && src[j] != '*')
		j++;
	while (i < j)
	{
		if (src[i] != str[i])
			return (false);
		i++;
	}
	return (true);
}

static void	is_prefix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;
	
	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]))
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				errno = MEM_ALLOC;
				free_tab(res);
				return ;
			}
			count++;
		}
		i++;
	}
	res[count] = NULL;
	free_tab((*node)->content);
	(*node)->content = res;
}

static bool	find_suffix(char *str, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
		i++;
	while (src[j])
		j++;
	while (j > 0)
	{
		i--;
		j--;
		if (src[j] != str[i])
			return (false);
	}
	return (true);
}

static void	is_suffix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;
	
	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_suffix(current_dir[i], &(*node)->content[0][1]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_suffix(current_dir[i], &(*node)->content[0][1]))
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				errno = MEM_ALLOC;
				free_tab(res);
				return ;
			}
			count++;
		}
		i++;
	}
	res[count] = NULL;
	free_tab((*node)->content);
	(*node)->content = res;
}

static void	is_joker(t_token **node, char **current_dir)
{
	int		i;
	char	**res;
	
	i = 0;
	while (current_dir[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	i = 0;
	while (current_dir[i])
	{
		res[i] = ft_strdup(current_dir[i]);
		if (!res[i])
		{
			errno = MEM_ALLOC;
			free_tab(current_dir);
			return ;
		}
		i++;
	}
	res[i] = NULL;
	free_tab((*node)->content);
	(*node)->content = res;
}

static void	is_infix(t_token **node, char **current_dir)
{
	int		i;
	int		count;
	char	**res;
	int		suffix;
	
	i = 0;
	suffix = 0;
	count = 0;
	while ((*node)->content[0][suffix] && (*node)->content[0][suffix] != '*')
		suffix++;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]) && find_suffix(current_dir[i], &(*node)->content[0][suffix + 1]))
			count++;
		i++;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return ;
	}
	i = 0;
	count = 0;
	while (current_dir[i])
	{
		if (find_prefix(current_dir[i], (*node)->content[0]) && find_suffix(current_dir[i], &(*node)->content[0][suffix + 1]))
		{
			res[count] = ft_strdup(current_dir[i]);
			if (!res[count])
			{
				errno = MEM_ALLOC;
				free_tab(res);
				return ;
			}
			count++;
		}
		i++;
	}
	res[count] = NULL;
	free_tab((*node)->content);
	(*node)->content = res;
}

static bool	make_wildcard(t_token **node)
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
	if (errno == MEM_ALLOC)
		return (false);
	free_tab(current_dir);
	return (true);
}

void	handle_wildcard(t_token **head, bool flag)
{
	t_token	*tmp;

	if (!head || !*head || !flag)
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
