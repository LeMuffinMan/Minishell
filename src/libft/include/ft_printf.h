/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:09:59 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 01:05:22 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

int		ft_printf(const char *format, ...);
int		ft_print_un(unsigned long long int n, char *base);
int		ft_print_ptr(void *n, char *base);
void	ft_put_base(unsigned long long n, char *base);
int		ft_num_len(long long int n, int baselen);
int		ft_print_int(long long int n, char *base);
int		ft_putstr(char *str);
int		ft_unnum_len(unsigned long long int n, int baselen);
int		ft_putchar(char c);

#endif