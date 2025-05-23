/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 22:58:33 by asinsard         ###   ########lyon.fr   */
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

static bool	lexing_and_tokenize(char *line, t_token **token,
								t_var *list_env, bool *flag)
{
	t_lexer	*lexer;

	lexer = NULL;
	if (!parse_line(line, &lexer))
		return (false);
	*flag = check_op_and_parenthesis(lexer);
	display_lexer(lexer, DEBUG);
	while (lexer->next)
	{
		if (!add_back(token, lexer->arg))
			return (false);
		lexer = lexer->next;
	}
	if (!add_back(token, lexer->arg))
		return (false);
	free_lexer(lexer);
	assign_token(token, list_env, *flag);
	return (true);
}

t_tree	*parse(char *line, t_var *list_env, t_lists *lists)
{
	t_token	*token;
	t_tree	*root;
	bool	flag;

	token = NULL;
	root = NULL;
	if (!lexing_and_tokenize(line, &token, list_env, &flag))
		return (NULL);
	error_one_quote(&token);
	error_one_parenthesis(&token);
	handle_wildcard(&token, flag);
	display_list(token, DEBUG);
	concat_args(&token, list_env, flag, lists);
	if (!handle_here_doc(&token))
		return (NULL);
	display_list(token, DEBUG);
	add_to_root(&token, &root);
	if (!root)
		free_parse(token, NULL, 0);
	display_ast(root, DEBUG);
	return (root);
}


