/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/17 13:03:34 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include "here_doc.h"
#include "structs.h"
#include <unistd.h>
#include <stdio.h>

t_tree	*parse(char *line, char **envp, t_var *list_env)
{
	t_token	*token;
	t_lexer	*lexer;
	t_tree	*root;
	bool	flag;

	token = NULL;
	lexer = NULL;
	root = NULL;
	parse_line(line, &lexer);
	flag = check_op_and_parenthesis(lexer);
	display_lexer(lexer, DEBUG);
	while (lexer->next)
	{
		add_back(&token, lexer->arg);
		lexer = lexer->next;
	}
	add_back(&token, lexer->arg);
	free_lexer(lexer, NULL, 0);
	assign_token(&token, envp, list_env, flag);
	display_list(token, DEBUG);
	concat_args(&token, list_env, envp);
	if (!handle_here_doc(&token))
		free_parse(token, "Problem with here_doc creation", MEM_ALLOC);
	add_to_root(&token, &root);
	display_list(token, DEBUG);
	if (!root)
		free_parse(token, NULL, 0);
	display_ast(root, DEBUG);
	return (root);
}


