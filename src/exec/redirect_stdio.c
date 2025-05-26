/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdio.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:25:23 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:25:48 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <unistd.h>

int	redirect_stdio_right_redirect(t_tree **ast, t_lists *lists)
{
	t_tree	*left;
	t_tree	*right;

	left = (*ast)->left;
	right = (*ast)->right;
	if (left && (left->token->token == DIREC && left->token->error == 127))
		lists->exit_code = error_cmd(left->token->content[0], 127);
	if (right && (right->token->token == DIREC && right->token->error == 127))
		lists->exit_code = error_cmd(right->token->content[0], 127);
	if (left && (left->token->token == R_IN || left->token->token == APPEND
			|| left->token->token == TRUNC || left->token->token == HD))
		lists->exit_code = redirect_stdio(&left, lists);
	if (errno == ENOMEM)
		return (errno);
	if (lists->exit_code == 0 && right && (right->token->token == R_IN
			|| right->token->token == APPEND || right->token->token == TRUNC
			|| right->token->token == HD))
		lists->exit_code = redirect_stdio(&right, lists);
	if (errno == ENOMEM)
		return (errno);
	return (0);
}

int	redirect_stdio_right(t_tree **ast, t_lists *lists)
{
	t_tree	*left;
	t_tree	*right;

	left = (*ast)->left;
	right = (*ast)->right;
	redirect_stdio_right_redirect(ast, lists);
	if (errno == ENOMEM)
		return (errno);
	if (lists->exit_code == 0 && left && (left->token->token == CMD
			|| left->token->token == BUILT_IN))
		lists->exit_code = exec_cmd(&left, lists);
	if (errno == ENOMEM)
		return (errno);
	if (lists->exit_code == 0 && right && (right->token->token == CMD
			|| right->token->token == BUILT_IN))
		lists->exit_code = exec_cmd(&right, lists);
	if (errno == ENOMEM)
		return (errno);
	return (0);
}

int	redirect_stdio(t_tree **ast, t_lists *lists)
{
	char	*file;

	if ((*ast)->token->token == HD)
		file = (*ast)->token->content[2];
	else
		file = (*ast)->token->content[1];
	lists->exit_code = file_check(file, (*ast)->token->token,
			(*ast)->token->error);
	if (lists->exit_code != 0)
		return (lists->exit_code);
	lists->exit_code = open_dup2_close(file, (*ast)->token->token);
	if (lists->exit_code == -1)
		return (ENOMEM);
	redirect_stdio_right(ast, lists);
	if (errno == ENOMEM)
		return (errno);
	if ((*ast)->token->token == HD)
		unlink(file);
	return (lists->exit_code);
}
