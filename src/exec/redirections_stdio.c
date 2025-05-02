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

//renvoie 1 pour un dossier
//renvoie 0 autrement
int is_a_directory(char *name)
{
	struct stat file_infos;

	if (stat(name, &file_infos) == -1)
	{
		//perror
		return (-1);
	}
	if (S_ISDIR(file_infos.st_mode)) //maccro qui renvoie un booleen true si c'est un dossier
		return (1); // si on a un whoami > file > dir > file je veux aller jusqu'au file avant de renvoyer l'erreur ?
	return (0);
}

int file_check(char *file, t_type type)
{
	if (type == R_IN || HD)
	{
		if(access(file, F_OK) == -1)
			return (print_error_file_opening(file, ": No such file or directory\n"));
		if (access(file, R_OK) == -1)
			return (print_error_file_opening(file, ": Permission denied\n"));
	}
	if (is_a_directory(file))
		return(print_error_file_opening(file, ": Is a directory\n"));
	if (type == APPEND || TRUNC)
	{
		if (access(file, W_OK) == -1)
			return (print_error_file_opening(file, ": Permission denied\n"));
	}
	return (0);
}

int open_dup2_close(char *file, t_type type)
{
	int fd;

	if (type == R_IN || type == HD)
		fd = open(file, O_RDONLY);
	else if (type == APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (type == TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC,	0644);
	if (fd == -1)
		return(-1);
	if (type == R_IN || type == HD)
		dup2(fd, STDIN_FILENO);
	else if (type == APPEND || type == TRUNC)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int redirect_stdio(t_tree **ast, t_var **env, int origin_fds[2])
{
	t_tree *left;
	t_tree *right;
	int exit_code;

	left = (*ast)->left;
	right = (*ast)->right;
	//doute pour ce if, on devrait le faire au parsing ?
	//dans tous les cas, avec juste un input "<" je trouve un synbole aleatoire dans content[1]
	if (!(*ast)->token->content[1])
		return(print_error_file_opening("", "syntax error\n"));
	exit_code = file_check((*ast)->token->content[1], (*ast)->token->token);
	if (exit_code != 0)
		return(exit_code);
	exit_code = open_dup2_close((*ast)->token->content[1], (*ast)->token->token);
	if (exit_code == -1)
		return (-1);//on stop la chaine de redirections
	if (left && (left->token->token == R_IN || left->token->token == APPEND || left->token->token == TRUNC || left->token->token == HD))
		exit_code = redirect_stdio(&left, env, origin_fds);
	if (exit_code == 0 && right && (right->token->token == R_IN || right->token->token == APPEND || right->token->token == TRUNC || right->token->token == HD))
		exit_code = redirect_stdio(&right, env, origin_fds);
	if (exit_code == 0 && left && (left->token->token == CMD || left->token->token == BUILT_IN))
		exit_code = exec_cmd(&left, env, origin_fds);
	if (exit_code == 0 && right && (right->token->token == CMD || right->token->token == BUILT_IN))
		exit_code = exec_cmd(&right, env, origin_fds);
	return (exit_code);
}

