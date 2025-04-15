/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:18:17 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "token.h"
#include "list.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int get_pipe(int input, int output)
{
	int fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipex: pipe error: ");
		//on free tout !
		exit(errno);
	}
	input = fd[0];
	output = fd[1];
	return (0);
}

int redirect_in(int fd, char *filename, t_type mode)
{
	int file;
	//on ouvre le fichier
	//on dup sur son fd
	//
	//auelaue chose a close ici ?
		/* close((*tmp)->prev->fd[1]); */
	file = open(filename, O_RDONLY); //pas besoin de perm ici ?
	if (file < 0)
	{
		//erreur a imprimer !
	}
	if (dup2(file, STDIN_FILENO) == -1)
	{
		//gerer l'erreur de dup2
	}
	fd = file;
	close(file);
	return (0);
}

int redirect_out(int fd, char *filename, t_type mode)
{
	int file;
	//on ouvre le fichier
	//on dup sur son fd
	//
	//auelaue chose a close ici ?
		/* close((*tmp)->prev->fd[1]); */
	if (mode == TRUNC)
		file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		file = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file < 0)
	{
		//erreur a imprimer !
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		//gerer l'erreur de dup2
	}
	fd = file;
	close(file);
	return (0);
}

//DANS LE MAIN
//if ast->token == O_AND
//{
	//if (!exec_ast(env, ast))
	//{
	//  update env
	//  exit code = exec_ast(env, ast)
	//}
	//else
	//{
	// 	
	//}
// 		

int temp_exec(t_var **env, t_tree *ast)
{
	int fd[2];
	t_pid status;
	pid_t pid;
	char **env_tmp;

	env_tmp = lst_to_array(env);
	if (ast->prev->token->token == PIPE)
	{
		get_pipe(fd);
		pid = fork();
		if (pid == 0)
			execve(ast->token->content[0], ast->token->content[1], env_tmp);
		pid = fork();
		if (pid == 0)
			execve(ast->token->content[0], ast->token->content[1], env_tmp);
	}
	if (ast->token->token == BUILT_IN) //tous les built in dans le parent ?
	{
		if (!ft_strncmp(content[0], "echo", 5))
			return (builtin_echo(arg));
		else if (!ft_strncmp(content[0], "cd", 3))
			return (builtin_cd(arg, env));
		else if (!ft_strncmp(content[0], "pwd", 4))
			return (builtin_pwd());
		else if (!ft_strncmp(content[0], "export", 7))
			return (builtin_export(env, arg));
		else if (!ft_strncmp(content[0], "unset", 6))
			return (builtin_unset(env, arg));
		else if (!ft_strncmp(content[0], "env", 4))
			return (builtin_env(env));
		else if (!ft_strncmp(content[0], "exit", 5))
			return (builtin_exit(arg, env));
	}
}

//Besoin du fichier vers le(s)quel(s) rediriger
//si echo "hello" > file1 > file2 > file3
//il me faudrait un char **avec tous les fichiers sur lequels appliquer lq mm fct truncate
int exec_ast(t_var **env, t_tree *ast)
{
	int exit_code; //int ?
	t_ast *tmp;

	//1 on parcourt tout l'arbre pour trouver les here_docs ?
		//si on en trouve : on les ouvre tous un par un, LIM par LIM on les ferme
	  //on ecrit hd1 hd2 hd3 hd4 dans tmp
	  //les redirs in auront le nom du hd a portee ?
		//$HOME est un limiter possible

		//si on a un && : on execute a droite que si succes
	if (ast->token == O_AND)
	{
		//on checker d'abord a gauche des expands a faire ? les deux en meme temnps ?
		exit_code = exec_ast(env, ast->left);
		if (exit_code == 0) 
			exit_code = exec_ast(env, ast->right); 
	}
	//si on a un || : on execute a droite que si fail
	else // on peut que avoir && || en haut ?
	{
		exit_code = exec_ast(env, ast->left);
		if (exit_code != 0)
			exit_code = exec_ast(env, ast->right);
	}
	//si on arrive la, c'est qu'on veut aller jusqu'a la premiere commande a executer le + en bas a gauche possible
	if (ast->left)
		exec_ast(env, ast->left);
	if (ast->right)
		exec_ast(env, ast->right);
	//
	//si on arrive la , c'est qu'on est tout en bas : une cmd a exec
	//	
	temp_exec()
	//On cherche si on a des redirections a faire 
	/* tmp = ast; */
	/* while (tmp->prev->token->token != O_AND && tmp->prev->token->token != O_OR) */
	/* { */
	/* 	if (tmp->token->token == PIPE) */
	/* 	{ */
	/* 		redirect_pipe(ast->left->) */
	/* 	} */
	/* 	tmp = tmp->prev; */
	/* } */
	// on a finit les redir on passe aux pipes ?

	//echo "coucou" > file && cat file
	//1 on cherche si ya un code d'erreur deja donne par Abraham ?
		//si oui : on execute l'erreur 
		//sinon :
	//4 On cherche la cmd a executer en descendant dans l'arbre
	//
	//ici on est arrive en bas
	//
	//execute_cmd(env, ast);
	//execute cmd :
		//cas 1 : on fork 
		//cas 2 : parent (cd, unset, export, env, exit ?)
	//wildcard ?

	//free tous 
	if (ast->left && ast->right == NULL)
	{
		//free node 
	}
	return (0);

}


// a tester
/* if (arg[0]) */
/* 	update_last_cmd_var(env, arg[0]); */
/* faire un switch case et y mettre les returns */
// 1 : ouvrir et remplir tous les here_docs
// 2 : open et rediriger tous les outputs / inputs vers / pour une cmd
// 3 : checker si c'est un builtin ou une cmd
// parent pour les builtins
// enfants pour les cmds ?
// 4 Wait
// 5 Mettre a jour l'environnement
int	exec(char **arg, t_var **env)
{
	if (!*arg)
		return (0);
	else if (!ft_strncmp(arg[0], "echo", 5))
		return (builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (builtin_cd(arg, env));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (builtin_pwd());
	else if (!ft_strncmp(arg[0], "export", 7))
		return (builtin_export(env, arg));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (builtin_unset(env, arg));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (builtin_env(env));
	else if (!ft_strncmp(arg[0], "exit", 5))
		return (builtin_exit(arg, env));
	else
		ft_putstr_fd("child process will run any other cmd\n", 1);
	return (0);
}
