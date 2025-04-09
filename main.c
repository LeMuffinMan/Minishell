/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 18:15:57 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "stack.h"
#include "token.h"
#include "display.h"
#include "libft.h"
#include <unistd.h>

int	main(int ac, char **av, char **envp)
{
	t_token	*token;
	t_lexer	*lexer;
	int		i;

	i = 0;
	if (ac != 2)
		ft_error("Usage: ./parsing arg\n", E_IO);
	token = NULL;
	lexer = NULL;
	parse_line(av[1], &lexer);
	while (lexer->next)
	{
		add_back(&token, lexer->arg);
		lexer = lexer->next;
	}
	add_back(&token, lexer->arg);
	// display_lexer(lexer);
	free_lexer(lexer, NULL, 0);
	assign_token(&token, envp);
	concat_args(&token);
	display_list(token);
	free_parse(token, NULL, 0);
	return (0);
}
