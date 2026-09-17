/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:25:07 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 02:48:24 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
#include <stdlib.h>

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

// bash lists what a wildcard matches in alphabetical order. readdir makes
// no promise about the order it hands the entries back in.
static void	sort_names(char **names, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(names[j], names[j + 1]) > 0)
			{
				tmp = names[j];
				names[j] = names[j + 1];
				names[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**extract_current_dir(void)
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
	sort_names(res, i);
	return (res);
}

int	which_case_of_wildcard(char *str)
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

void	assign_wildcard(t_token **node, char **res, int count)
{
	res[count] = NULL;
	if (res[0])
	{
		free_tab((*node)->content);
		(*node)->content = res;
	}
	else
		free_tab(res);
}
