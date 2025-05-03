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

//a tester !
int update_last_arg_var(t_var **env, char **content)
{
	t_var *tmp;
	int i;

	tmp = *env;
	while (tmp)
	{
		if(!ft_strncmp(tmp->key, "_", 2) && tmp->env == 0)
		{
			i = 0;
			while(content[i])
				i++;
			if (tmp->value) //mettre cette protection partout !
				free(tmp->value);
			tmp->value = ft_strdup(content[i]);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	builtins(char **arg, t_var **env, t_tree **ast, int origin_fds[2])
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
		return (builtin_env(env, arg));
	else if (!ft_strncmp(arg[0], "exit", 5))
		return (builtin_exit(arg, env, ast, origin_fds));
	/* else if (!ft_strncmp(arg[0], "source", 7)) */
	/* 	return (builtin_source(arg, env, ast)); */
	else
		return (1);
	//ajouter source
		//vide les var exportee depuis minishellrc
		//relance l'init du minishell_rc
}

int	exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes, int origin_fds[2])
{
	pid_t left_pid;
	pid_t right_pid;
	int		pipefd[2];
	int exit_code;

	add_pipe(pipefd, pipes);
	left_pid = fork();
	if (left_pid < 0)
	{
		// error
	}
	if (left_pid == 0)
	{
		
		close(origin_fds[0]);
		origin_fds[0] = -1;
		close(origin_fds[1]);
		origin_fds[1] = -1;
		close(pipefd[0]);
		if ((*pipes)->next)
		{
			dup2((*pipes)->next->fd[0], STDIN_FILENO);
			close((*pipes)->next->fd[0]);
			free((*pipes)->next);
			(*pipes)->next = NULL;
		}
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		free_pipes(pipes);
		exit_code = exec_ast(&((*ast)->left), env, origin_fds);
		free_list(env);
    free_parse((*ast)->token, NULL, 0);
		free_tree(ast);
		exit(exit_code);
	}
	else
	{
		if ((*pipes)->next)
		{
			close((*pipes)->next->fd[0]);
			free((*pipes)->next);
			(*pipes)->next = NULL;
		}
		close(pipefd[1]);
	}
	if ((*ast)->right && (*ast)->right->token->token == PIPE)
	{
		//attendre avant ?
		exit_code = exec_pipe(&((*ast)->right), env, pipes, origin_fds);
		exit_code = wait_children(left_pid, left_pid);
		return (exit_code);
	}
	else
	{
		right_pid = fork();
		if (right_pid < 0)
		{
			//error
		}
		if (right_pid == 0)
		{
			close(origin_fds[0]);
			origin_fds[0] = -1;
			close(origin_fds[1]);
			origin_fds[1] = -1;
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			free_pipes(pipes);
			exit_code = exec_ast(&((*ast)->right), env, origin_fds);
			free_list(env);
    	free_parse((*ast)->token, NULL, 0);
			free_tree(ast);
			exit(exit_code);
		}
		else
		{
			close(pipefd[0]);
			free_pipes(pipes);
			exit_code = wait_children(right_pid, left_pid);
			update_last_arg_var(env, (*ast)->token->content);
			return(exit_code);
		}
	}
	return (1);
}

/* #include <stdio.h> */
//BUILT_IN : PARENT
//CMD : CHILD
int	exec_cmd(t_tree **ast, t_var **env, int origin_fds[2])
{
	char	**strings_env;
	pid_t	pid;
	int exit_code;
	char *s;
	char *tmp;

	/* expand_cmd_sub((*ast)->token->content, env); */
	//les builtins ne mettent pas a jour la variable _ !!
	if ((*ast)->token->token == BUILT_IN)
	{
		exit_code = builtins((*ast)->token->content, env, ast, origin_fds);
		return (exit_code);
	}
	if (is_a_directory((*ast)->token->content[0]))
	{
		if (!ft_strncmp((*ast)->token->content[0], ".", 2))
		{
			ft_putstr_fd("minishell: .: filename argument required\n.: usage: . filename [arguments]\n", 2);
			return(2);
		}
		if (!ft_strncmp((*ast)->token->content[0], "..", 3))
		{
			ft_putstr_fd("minishell: ..: command not found\n", 2);
			return(127);
		}
		s = ft_strjoin("minishell: ", (*ast)->token->content[0]);
		tmp = s;
		s = ft_strjoin(s, ": Is a directory\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
		return (126);
	}

	if ((*ast)->token->token == CMD)
	{
		pid = fork();
		if (pid == -1)
			return(-1);
		if (pid == 0)
		{
			if (origin_fds[0] > 2)
				close(origin_fds[0]);
			if (origin_fds[1] > 2)
				close(origin_fds[1]);
			strings_env = lst_to_array(env);
			execve((*ast)->token->content[0], (*ast)->token->content,
				strings_env);
			perror("execve");
			free_array(strings_env);
      free_parse((*ast)->token, NULL, 0);
			free_tree(ast);
			free_list(env);
			exit(1);
		}
		else
		{
			exit_code = wait_children(pid, pid);
			update_env(env);
			update_last_arg_var(env, (*ast)->token->content);
			return (exit_code);
		}
	}
	return (1); //on ne devrait pas arriver ici
}

int	exec_ast(t_tree **ast, t_var **env, int origin_fds[2])
{
  t_pipe *pipes;
  int exit_code;

  pipes = NULL;
  exit_code = 0;
  if (!*ast)
  	return(127);
  if ((*ast)->token->error == 2)
  {
  	ft_putendl_fd((*ast)->token->content[0], 2);
  	return ((*ast)->token->error);
  }
	if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC || (*ast)->token->token == HD) //HD dans l'arbre ? 
		return(redirect_stdio(ast, env, origin_fds));
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, &pipes, origin_fds));
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)
	{
		exit_code = exec_cmd(ast, env, origin_fds);
		return (exit_code);
	}
	//errors
	/* if (!ft_strncmp((*ast)->token->content[0], ":", 2)) */
	/* 		return(0); */
	if ((*ast)->token->error == 127 || (*ast)->token->error == 126)
		return (error_cmd((*ast)->token->content[0], (*ast)->token->error));
	//Ultrabonus
		//un token Alias
	  //un token shell_func
	  //un token substitution cmd ?
			//on ne l'expand pas jusqu'au dernier moment, et on execute le contenu des parentheses 
	return (exit_code);
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

