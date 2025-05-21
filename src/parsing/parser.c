/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 22:35:47 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include "here_doc.h"
#include "structs.h"
#include "wildcard.h"
#include <unistd.h>
#include <stdio.h>

t_tree	*parse(char *line, char **envp, t_var *list_env, t_lists *lists)
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
	error_one_quote(&token);
	error_one_parenthesis(&token);
	handle_wildcard(&token, flag);
	display_list(token, DEBUG);
	concat_args(&token, list_env, envp, flag, lists);
	if (!handle_here_doc(&token))
		free_parse(token, "Problem with here_doc creation", MEM_ALLOC);
	display_list(token, DEBUG);
	add_to_root(&token, &root);
	if (!root)
		free_parse(token, NULL, 0);
	display_ast(root, DEBUG);
	return (root);
}


