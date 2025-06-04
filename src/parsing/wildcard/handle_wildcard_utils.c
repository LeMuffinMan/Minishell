/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:25:07 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/04 18:20:50 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
#include <stdlib.h>
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

char	**extract_current_dir(void)
{
	int		i;
	char	**res;

	i = 0;
	handle_current_dir(&i, NULL);
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
	{
		errno = MEM_ALLOC;
		return (NULL);
	}
	i = 0;
	if (!handle_current_dir(&i, &res))
	{
		free_tab(res);
		return (NULL);
	}
	res[i] = NULL;
	return (res);
}

int	which_case_of_wildcard(char *str)
{
	int	sort_of;
	int	i;

	i = 0;
	sort_of = 0;
	while (str[i])
	{
		if (str[i] == '*')
		{
			if (i > 0)
			{
				if (str[i + 1] && str[i + 1] != '*')
					sort_of = INFIX;
				else
					sort_of = PREFIX;
			}
			else if (str[i + 1] && str[i + 1] != '*')
				sort_of = SUFFIX;
			else
				sort_of = JOKER;
			return (sort_of);
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

int	compare_content(char *s1, char *s2)
{
	int		i;
	char	c1;
	char	c2;

	i = 0;
	while (s1[i] && s2[i]) // c'etait un || 
	{
		c1 = s1[i];
		c2 = s2[i];
		if (s1[i] >= 'A' && s1[i] <= 'Z')
			c1 = s1[i] + 32;
		if (s2[i] >= 'A' && s2[i] <= 'Z')
			c2 = s2[i] + 32;
		if (c1 != c2)
			return (c1 - c2);
		i++;
	}
	//Si on tombe sur un char de s1 ou s2 : inexistant : on doit renvoyer une valeur inverse  
	return (s1[i] - s2[i]);
}
