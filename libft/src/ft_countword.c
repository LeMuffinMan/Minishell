/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countword.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:06:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:06:47 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_countword(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*str)
	{
		if (!ft_isspace(*str) && i == 0)
		{
			i = 1;
			count++;
		}
		else if (ft_isspace(*str) && i != 0)
			i = 0;
		str++;
	}
	return (count);
}
