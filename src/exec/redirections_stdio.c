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
#include <sys/stat.h>

// attention si c'est le proc parrent !! Redup avant de rendre le prompt !!!
int	open_dup2_close(t_tree **ast, t_type type, int fd[2])
{
	struct stat file_infos;

	if (stat((*ast)->token->content[1], &file_infos) == -1)
	{
		//error
	}
	if (S_ISDIR(file_infos.st_mode)) //maccro qui renvoie un booleen true si c'est un dossier
		return (1); // si on a un whoami > file > dir > file je veux aller jusqu'au file avant de renvoyer l'erreur ?
	if (type == R_IN)
	{
		fd[0] = open((*ast)->token->content[1], O_RDONLY);
		if (fd[0] == -1)
		{
			// error
		}
		/* dup2(fd[0], STDIN_FILENO); */
		/* close(fd[0]); */
	}
	else if (type == APPEND)
	{
		fd[1] = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_APPEND,
				0644);
		if (fd[1] == -1)
		{
			// error
		}
		/* dup2(fd[1], STDOUT_FILENO); */
		/* close(fd[1]); */
	}
	else if (type == TRUNC)
	{
		fd[1] = open((*ast)->token->content[1], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
		if (fd[1] == -1)
		{
			// error
		}
		/* dup2(fd[1], STDOUT_FILENO); */
		/* close(fd[1]); */
	}
	return (0);
}

int	redirect_stdio(t_tree **ast, t_var **env)
{
	t_tree	*left;
	t_tree	*right;
	int exit_code;
	char *s;
	char *tmp;
	int fd[2];
	/* int stdin_fd; */
	/* int stdout_fd; */

	left = (*ast)->left;
	fd[0] = -1;
	fd[1] = -1;
	/* if (left) */
	/* { */
	/* 	stdin_fd = dup(STDIN_FILENO); */
 /*  	stdout_fd = dup(STDOUT_FILENO); */
	/* } */
	right = (*ast)->right;
	exit_code = open_dup2_close(ast, (*ast)->token->token, fd);
	if (exit_code == 1) // Is a dir
	{
		s = ft_strjoin("minishell: ", (*ast)->token->content[1]);
		tmp = s;
		s = ft_strjoin(s, ": Is a directory\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
	}
	else if (exit_code == -1)
	{
		/* un open ou un dup qui a foire  */
	}
	if (right)
		exit_code = open_dup2_close(&right, right->token->token, fd);
	/* if (!left) // cas impossible ? */
	/* 	return (exit_code); */

	#include <stdio.h>
	dprintf(2, "STDIN = %d\n", STDIN_FILENO);
	dprintf(2, "STDOUT = %d\n", STDOUT_FILENO);
	printf("left = %p\n", (*ast)->left);
	printf("right = %p\n", (*ast)->right);
	if (left)
	{
		exit_code = exec_cmd(&left, env, fd);
	}
	return (exit_code);
}
