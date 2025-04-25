/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_stdio.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:47:57 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 17:02:03 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "libft.h"
#include "list.h"
#include "exec.h"

// attention si c'est le proc parrent !! Redup avant de rendre le prompt !!!
int	open_dup2_close(t_tree **ast, t_type type)
{
	int	fd;

	if (type == R_IN)
	{
		fd = open((*ast)->token->content[1], O_RDONLY);
		if (fd == -1)
		{
			// error
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (type == APPEND)
	{
		fd = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_APPEND,
				0644);
		if (fd == -1)
		{
			// error
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (type == TRUNC)
	{
		fd = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
		if (fd == -1)
		{
			// error
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	redirect_stdio(t_tree **ast, t_var **env)
{
	t_tree	*left;
	t_tree	*right;
	int stdin_fd;
	int stdout_fd;
	int exit_code;

	stdin_fd = dup(STDIN_FILENO);
  stdout_fd = dup(STDOUT_FILENO);
	left = (*ast)->left;
	right = (*ast)->right;

		#include <stdio.h>
		printf("ici\n");
	if (open_dup2_close(ast, (*ast)->token->token))
	{
		// Error
	}
	if (right)
		exec_ast(&right, env);
	if (!left)
		return (0);
	else
	{
		exit_code = exec_ast(&left, env);
    dup2(stdin_fd, STDIN_FILENO);
    dup2(stdout_fd, STDOUT_FILENO);
		return (exit_code);
	}
}
