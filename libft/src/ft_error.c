/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:23:54 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 01:38:40 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_error(const char *s, int error)
{
	if (!s)
		return ;
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(s, 2);
	if (error != 0)
		exit(error);
}
