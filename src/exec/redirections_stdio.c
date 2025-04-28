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
	int fd[2];

	if (type == R_IN)
	{
		fd[0] = open((*ast)->token->content[1], O_RDONLY);
		if (fd[0] == -1)
		{
			// error
		}
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else if (type == APPEND)
	{
		fd[1] = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_APPEND,
				0644);
		if (fd[1] == -1)
		{
			// error
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	else if (type == TRUNC)
	{
		fd[1] = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
		if (fd[1] == -1)
		{
			// error
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	return (0);
}

int redirect_stdio(t_tree **ast, t_var **env)
{
	t_tree *left;
	t_tree *right;
	int exit_code;
	char *s;
	char *tmp;

	left = (*ast)->left;
	right = (*ast)->right;

	exit_code = open_dup2_close(ast, (*ast)->token->token);
	//si exit_code == 1 => c'est qu'on a un dossier milieu : on arrete l'exec 
	if (exit_code == 1)
	{
		s = ft_strjoin("minishell: ", (*ast)->token->content[1]);
		tmp = s;
		s = ft_strjoin(s, ": Is a directory\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
		return (1);
	}
	else if (exit_code == -1)
	{
		/* un open ou un dup qui a foire  */
		return (-1);
	}
	//si right existe : c'est qu'on a une chaine de redirections
	if (right)
		exit_code = redirect_stdio(&right, env);
	if (left)
	{
		exit_code = exec_cmd(&left, env);
	}
	return (exit_code);
}


/* int	redirect_stdio(t_tree **ast, t_var **env) */
/* { */
/* 	t_tree	*left; */
/* 	t_tree	*right; */
/* 	int exit_code; */
/* 	char *s; */
/* 	char *tmp; */
/**/
/* 	left = (*ast)->left; */
/* 	right = (*ast)->right; */
/* 	//Si j'ai une redirection vers RIEN */
/* 	if (!(*ast)->token->content[1]) */
/* 	{ */
/* 		if (right) */
/* 			exit_code = redirect_stdio(&right, env); */
/* 		else  */
/* 		{ */
/* 			ft_putstr_fd("minishell: syntax error\n", 2); */
/* 			return (2); */
/* 		} */
/* 	} */
/* 	exit_code = open_dup2_close(ast, (*ast)->token->token, fd); */
/* 	if (exit_code == 1) // Is a dir */
/* 	{ */
/* 		s = ft_strjoin("minishell: ", (*ast)->token->content[1]); */
/* 		tmp = s; */
/* 		s = ft_strjoin(s, ": Is a directory\n"); */
/* 		free(tmp); */
/* 		ft_putstr_fd(s, 2); */
/* 		free(s); */
/* 	} */
/* 	else if (exit_code == -1) */
/* 	{ */
/* 	} */
/* 	if (right) */
/* 		exit_code = open_dup2_close(&right, right->token->token, fd); */
/* 	if (left) */
/* 	{ */
/* 		exit_code = exec_cmd(&left, env); */
/* 	} */
/* 	return (exit_code); */
/* } */
