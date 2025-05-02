/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/29 13:30:37 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include "here_doc.h"
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

t_tree	*parse(char *line, char **envp)
{
	t_token	*token;
	t_lexer	*lexer;
	t_tree	*root;

	token = NULL;
	lexer = NULL;
	root = NULL;
	parse_line(line, &lexer);
	if (!lexer)
		return (NULL);
	/* display_lexer(lexer); */
	while (lexer->next)
	{
		add_back(&token, lexer->arg);
		lexer = lexer->next;
	}
	add_back(&token, lexer->arg);
	free_lexer(lexer, NULL, 0);
	assign_token(&token, envp);
	concat_args(&token);
	if (!handle_here_doc(&token))
		free_parse(token, "Problem with here_doc creation", MEM_ALLOC);
	add_to_root(token, &root, true);
	/* display_list(token); */
	/* display_ast(root); */
	return (root);
}
