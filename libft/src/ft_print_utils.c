/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:27:02 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 01:06:59 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	if (!str)
		return (write(1, "(null)", 6));
	write(1, str, ft_strlen(str));
	return (ft_strlen(str));
}

int	ft_unnum_len(unsigned long long int n, int baselen)
{
	int	len;

	len = 1;
	while (n >= (unsigned int)baselen)
	{
		n /= baselen;
		len++;
	}
	return (len);
}

int	ft_num_len(long long int n, int baselen)
{
	int	len;

	len = 1;
	if (n < 0)
		n *= -1;
	while (n >= baselen)
	{
		n /= baselen;
		len++;
	}
	return (len);
}
