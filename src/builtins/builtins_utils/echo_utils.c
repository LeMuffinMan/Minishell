/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:22:37 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:22:38 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_a_valid_echo_option(char *s)
{
	int	i;

	if (!ft_strncmp(s, "-n", 2))
	{
		i = 2;
		while (s[i])
		{
			if (s[i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	is_only_n(char *arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n' && arg[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}
