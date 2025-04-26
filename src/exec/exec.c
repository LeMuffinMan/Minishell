/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 17:03:34 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "list.h"
#include "pipe.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	builtins(char **arg, t_var **env, t_tree **ast)
{
	if (!*arg)
		return (1);
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
		return (builtin_exit(arg, env, ast));
	/* else if (!ft_strncmp(arg[0], "source", 7)) */
	/* 	return (builtin_source(arg, env, ast)); */
	else
		return (1);
	//ajouter source
		//vide les var exportee depuis minishellrc
		//relance l'init du minishell_rc
}
// free_env
// free_ast
// free_pipes
int	free_lists_and_exit(t_var **env, t_tree **ast, t_pipe **pipes)
{
	(void)env;
	(void)pipes;
	(void)ast;
	return (0);
}


int	exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes)
{
	pid_t	left_pid;
	pid_t	right_pid;
	t_tree	*left_branch;
	t_tree	*right_branch;
	int		pipefd[2];
	int exit_code;
	int stdin_fd;
	int stdout_fd;

	right_branch = (*ast)->right;
	left_branch = (*ast)->left;
	add_pipe(pipefd, pipes);
  //on veut sauvegarder le numero des fds originaux des lectures /ecritures
  stdin_fd = dup(STDIN_FILENO);
  stdout_fd = dup(STDOUT_FILENO);
	left_pid = fork();
	if (!left_pid)
	{
		// error
	}
	if (left_pid == 0)
	{
		//left ne vas jamais lire dnas le pipe qu'on vient de creer
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		free_pipes(pipes);
		return(exec_ast(&left_branch, env));
	}
	else
		close(pipefd[1]);
	right_pid = fork();
	if (!right_pid)
	{
		// error
	}
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		free_pipes(pipes);
		if (right_branch->token->token == PIPE)
			return(exec_pipe(&right_branch, env, pipes));
		else
			return (exec_ast(&right_branch, env));
	}
	else
	{
		close(pipefd[0]);
		free_pipes(pipes);
		/* pipes = NULL; */
    //si j'ai rediriger la lecture / ecriture du parent, 
    //il faut la reset correctement avant de rendre le prompt
    //il faut peut etre le faire encore plus tot ?
    dup2(stdin_fd, STDIN_FILENO);
    dup2(stdout_fd, STDOUT_FILENO);
		exit_code = wait_children(right_pid, left_pid);
		return(exit_code);
	}
	return (1);
}

//BUILT_IN : PARENT
//CMD : CHILD
int	exec_cmd(t_tree **ast, t_var **env)
{
	char	**strings_env;
	pid_t	pid;
	int exit_code;


	/* expand_cmd_sub((*ast)->token->content, env); */
	if ((*ast)->token->token == BUILT_IN)
	{
		exit_code = builtins((*ast)->token->content, env, ast);
		return (exit_code);
	}

	if ((*ast)->token->token == CMD)
	{
		pid = fork();
		if (pid == -1)
			return(-1);
		if (pid == 0)
		{
			strings_env = lst_to_array(env);
			execve((*ast)->token->content[0], (*ast)->token->content,
				strings_env);
			perror("execve");
			free_array(strings_env);
			free_tree(ast);
			free_list(env);
			exit(1);
		}
		else
		{
			exit_code = wait_children(pid, pid);
			/* free_tree(*ast); */
			/* free_list(env); */
			update_env(env);
			return (exit_code);
		}
	}
	return (1); //on ne devrait pas arriver ici
}

/* int	boolean_operators(t_tree **ast, t_var **env) */
/* { */
/* 	int		exit_code; */
/* 	t_tree	*tmp; */
/**/
/* 	exit_code = 0; */
/* 	if (find_expands(ast->left)) */
/* 		expand_variables(ast->left); */
/* 	tmp = (*ast)->left; */
/* 	exit_code = exec_ast(&tmp, env); */
/* 	if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0 */
/* 			&& (*ast)->token->token == O_OR)) */
/* 	{ */
/* 		if (find_expands(ast->right)) */
/* 			expand_variables(ast->right); */
/* 		tmp = (*ast)->right; */
/* 		return (exec_ast(&tmp, env, pipes)); */
/* 		return (1); */
/* 	} */
/* 	else */
/* 		return (exit_code); */
/* } */

int	exec_ast(t_tree **ast, t_var **env)
{
  t_pipe *pipes;

  pipes = NULL;

	//O_AND VONT VIRER 
	/* if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR) */
	/* 	return (boolean_operators(ast, env, pipes, pids)); */
	if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC) 
		redirect_stdio(ast, env); // je devrai return ici ?
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, &pipes));
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)
		return (exec_cmd(ast, env));
	if ((*ast)->token->error == 127)
		return (error_cmd_not_found((*ast)->token->content[0]));
	if ((*ast)->token->error == 126)
		return (error_cmd_perm_denied((*ast)->token->content[0]));
		//un token VAR ?
	//Ultrabonus
		//un token Alias
	  //un token shell_func
	  //un token substitution cmd ?
			//on ne l'expand pas jusqu'au dernier moment, et on execute le contenu des parentheses 
	return (0);
}

//Gros debuggage 
//
//implementer le nouveau free de l'arbre 
//corriger si on fait un ctrl D dans le prompt : tout free
//idem pour exit : tout free
//
//Parsing ?
//whoami | cat | > file1 | uname
//doit afficher Linux et pas cmd not found !
//
					//TESTS
//
//PIPES
//
//whoami OK
//whoami | cat OK
//whoami | cat | cat OK
//whoami | cat | cat | cat OK
//
//REDIRS OUT
//whoami > file1 OK
//not_existing_cmd > file1 OK
//whoami > file1 > file2 OK
//whoami > file1 > file2 > file3 > file4 OK
//whoami >> file1 OK
//whoami > file1 >> file2 OK
//whoami >> file1 > file2 > file3
//whoami > file1 > file2 >> file3 KO
//
//REIDRS IN
//< file1 whoami KO
//< file1 cat : boucle infinie KO
//
//PIPE + redirs out
//whoami | cat | cat > file1 | cat > file1 > file2 | cat KO
//
//PIPE + redirs in
//TODO

//Mettre 1024 en limite de pipes !!
//
//gros BUG !
/* [Minishell]$ whoami | file1 */
/* mimishell: file1: Permission denied */
/* [Minishell]$ oelleaum */
/* mimishell: oelleaum: command not found */
/* [Minishell]$ 2004h[Minishell]$ */

// exec_ast.c
/* int exec_ast(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int redirect_stdio(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes); */

// pipe_utils.c
/* int get_fds_for_pipe(t_tree *ast); */
/* int add_pipe(int fd[2], t_pipe **pipes); */
/* int free_pipes(t_pipe **pipes); */

// exec_utils.c
/* int expand_variables(t_tree **ast); */
/* int find_expands(t_tree *ast) */
/* int free_lists_and_exit(t_var **env, t_tree **ast, t_pipe **pipes) */

/* char *enum_to_string(t_type enumValue) */
/* { */
/* 	char *s; */
/**/
/*   if (enumValue == CMD) */
/*       s = ft_strdup("CMD"); */
/*   else if (enumValue == BUILT_IN) */
/*       s = ft_strdup("BUILT_IN"); */
/*   else if (enumValue == APPEND) */
/*       s = ft_strdup("APPEND"); */
/*   else if (enumValue == D_QUOTE) */
/*       s = ft_strdup("D_QUOTE"); */
/*   else if (enumValue == HD) */
/*       s = ft_strdup("HD"); */
/*   else if (enumValue == LIM) */
/*       s = ft_strdup("LIM"); */
/*   else if (enumValue == O_AND) */
/*       s = ft_strdup("O_AND"); */
/*   else if (enumValue == O_OR) */
/*       s = ft_strdup("O_OR"); */
/*   else if (enumValue == PIPE) */
/*       s = ft_strdup("PIPE"); */
/*   else if (enumValue == R_IN) */
/*       s = ft_strdup("R_IN"); */
/*   else if (enumValue == S_QUOTE) */
/*       s = ft_strdup("S_QUOTE"); */
/*   else if (enumValue == TRUNC) */
/*       s = ft_strdup("TRUNC"); */
/*   else if (enumValue == WILDCARD) */
/*       s = ft_strdup("WILDCARD"); */
/*   else */
/*       s = ft_strdup("UNKNOWN"); */
/*   return (s); */
/* } */
/**/
