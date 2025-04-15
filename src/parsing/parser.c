/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/15 18:18:20 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include <unistd.h>
#include <readline/readline.h>
#include <stdio.h>

t_tree	*parse(char *line, char **envp)
{
	t_token	*token;
	t_lexer	*lexer;
	t_tree	*root;
	/* int		i; */

	/* i = 0; */
	/* if (ac != 2) */
	/* 	ft_error("Usage: ./parsing arg\n", E_IO); */
	token = NULL;
	lexer = NULL;
	parse_line(line, &lexer);
	// display_lexer(lexer);
	while (lexer->next)
	{
		add_back(&token, lexer->arg);
		lexer = lexer->next;
	}
	add_back(&token, lexer->arg);
	free_lexer(lexer, NULL, 0);
	assign_token(&token, envp);
	/* display_list(token); */
	concat_args(&token);
	/* display_list(token); */
	add_to_root(token, &root);
	/* print_ast(root, 20); */
	/* free_tree(root); */
	// free_parse(token, NULL, 0);
	return (root);
}
