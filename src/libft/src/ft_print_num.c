/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_num.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:10:53 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 01:06:45 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>

void	ft_put_base(unsigned long long n, char *base)
{
	int	baselen;

	baselen = ft_strlen(base);
	if (n >= (size_t)baselen)
		ft_put_base(n / baselen, base);
	write(1, &base[n % baselen], 1);
}

int	ft_print_ptr(void *n, char *base)
{
	int						numlen;
	unsigned long long int	res;

	numlen = 0;
	res = (unsigned long long int)n;
	if (res == 0)
	{
		numlen += write(1, "(nil)", 5);
		return (numlen);
	}
	else
	{
		numlen += write(1, "0x", 2);
		ft_put_base((unsigned long long)res, base);
		numlen += ft_unnum_len((unsigned long long int)res, ft_strlen(base));
	}
	return (numlen);
}

int	ft_print_un(unsigned long long int n, char *base)
{
	int	numlen;

	numlen = 0;
	ft_put_base(n, base);
	numlen += ft_unnum_len(n, ft_strlen(base));
	return (numlen);
}

int	ft_print_int(long long int n, char *base)
{
	int	numlen;

	numlen = 0;
	if (n < 0)
	{
		write(1, "-", 1);
		numlen += 1;
		n *= -1;
	}
	ft_put_base(n, base);
	numlen += ft_num_len(n, ft_strlen(base));
	return (numlen);
}
