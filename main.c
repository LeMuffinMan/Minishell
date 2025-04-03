/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 00:18:31 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "stdio.h"
#include "ft_printf.h"
#include <unistd.h>

int	main(int ac, char **av, char **envp)
{
	t_parse	*stack;
	t_parse	*tmp;
	int	i;
	(void)envp;

	i = 0;
	stack = NULL;
	tmp = NULL;
	while (++i != ac)
		add_back(&stack, av[i]);
	assign_token(&stack);
	display_list(stack);
	free_parse(stack);
	return (0);
}