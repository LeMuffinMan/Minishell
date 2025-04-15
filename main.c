/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/14 20:47:52 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include <unistd.h>

int	main(int ac, char **av, char **envp)
{
	t_token	*token;
	t_lexer	*lexer;
	t_tree	*root;
	int		i;

	i = 0;
	if (ac != 2)
		ft_error("Usage: ./parsing arg\n", E_IO);
	token = NULL;
	lexer = NULL;
	parse_line(av[1], &lexer);
	// display_lexer(lexer);
	while (lexer->next)
	{
		add_back(&token, lexer->arg);
		lexer = lexer->next;
	}
	add_back(&token, lexer->arg);
	free_lexer(lexer, NULL, 0);
	assign_token(&token, envp);
	display_list(token);
	concat_args(&token);
	parse_quote(&token);
	display_list(token);
	add_to_root(token, &root);
	display_ast(root);
	free_tree(root);
	free_parse(token, NULL, 0);
	return (0);
}
