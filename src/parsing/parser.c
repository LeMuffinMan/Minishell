/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:09:12 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/04 00:14:07 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "misc/display.h"
#include "tree.h"
#include "libft.h"
#include "quote.h"
#include "here_doc.h"
#include "structs.h"
#include "wildcard.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

bool	lexing_and_tokenize(char *line, t_token **token,
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
	if (errno == MEM_ALLOC)
		return (false);
	return (true);
}

t_tree	*parse(char *line, t_var *list_env, t_lists *lists)
{
	t_token	*token;
	t_tree	*root;
	bool	flag;

	token = NULL;
	root = NULL;
	if (!lexing_and_tokenize(line, &token, list_env, &flag)
		|| !error_one_quote(&token))
		return (NULL);
	if (!error_one_parenthesis(&token))
	{
		errno = MEM_ALLOC;
		return (NULL);
	}
	display_list(token, DEBUG);
	if (!concat_args(&token, list_env, flag, lists)
		||!handle_here_doc(&token, lists))
		return (NULL);
	display_list(token, DEBUG);
	add_to_root(&token, &root);
	if (!root)
		free_parse(token, NULL, 0);
	display_ast(root, DEBUG);
	return (root);
}
