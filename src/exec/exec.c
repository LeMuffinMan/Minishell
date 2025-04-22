/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:44:03 by oelleaum         ###   ########lyon.fr   */
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

int	builtins(char **arg, t_var **env, t_tree **ast, t_pipe **pipes)
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
		return (builtin_exit(arg, env, ast, pipes));
	else
		return (1);
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

/* int free_pipes(t_pipe **pipes) */
/* { */
/* 	t_pipe *tmp; */
/* 	t_pipe *last; */
/**/
/* 	tmp = *pipes; */
/* 	last = NULL; */
/* 	while (tmp) */
/* 	{ */
/* 		if (last) */
/* 			free(last); */
/* 		last = tmp; */
/* 		tmp = tmp->next;  */
/* 	} */
/* 	if (last) */
/* 		free(last); */
/* 	return (0); */
/* } */

int	exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes)
{
	pid_t	left_pid;
	pid_t	right_pid;
	t_tree	*left_branch;
	t_tree	*right_branch;
	int		pipefd[2];
	int exit_code;

	right_branch = (*ast)->right;
	left_branch = (*ast)->left;
	add_pipe(pipefd, pipes);
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
		return(exec_ast(&left_branch, env, pipes));
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
		return (exec_ast(&right_branch, env, pipes));
	}
	else
	{
		close(pipefd[0]);
		free_pipes(pipes);
		pipes = NULL;
		exit_code = wait_children(right_pid, left_pid);
		/* free_tree(*ast); */
		return(exit_code);
	}
	return (1);
}


// checker les fd open : des cas ou ca peut arriver je pense
int	exec_pipe_old(t_tree **ast, t_var **env, t_pipe **pipes)
{
	pid_t	left_pid;
	pid_t	right_pid;
	t_tree	*left_branch;
	t_tree	*right_branch;
	int		pipefd[2];
	int exit_code;

	right_branch = (*ast)->right;
	left_branch = (*ast)->left;
	add_pipe(pipefd, pipes);
	left_pid = fork();
	if (!left_pid)
	{
		// error
	}
	if (left_pid == 0)
	{
		//si je suis dans une pipeline, je branche ma lecture
		if ((*pipes)->next)
		{
			dup2((*pipes)->next->fd[0], STDIN_FILENO);
			close((*pipes)->next->fd[0]);
			free((*pipes)->next);
			(*pipes)->next = NULL;
			/* dprintf (2, "LEFT %s read in %d\n", (*ast)->left->token->content[0], (*pipes)->fd[0]); */
		}
		//vu que c'est a gauche : on veut ecrire, pas besoin de la lecture du pipe qu'on vient de creer 
		/* if (!(*pipes)->next) */
		/* 	close(pipefd[0]); */
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		/* dprintf(2, "LEFT %s write in %d\n", (*ast)->left->token->content[0], pipefd[1]); */
				//free_pipes ici ?
		//et donner NULL a pipes ?
		return(exec_ast(&left_branch, env, pipes));
	}
	else // parent de left
	{
		//si on est dans une pipeline on peut fermer et free le dernier
		if ((*pipes)->next)
		{
			close((*pipes)->next->fd[0]);
			(*pipes)->next = NULL;
			/* dprintf(2, "closing fd %d\n", tmp->fd[0]); */
			free((*pipes)->next);
			//si on etait dans une pipeline, le parent vient de fermer et free le pipe d'AVANT 
		}
		//on ne ferme pas la lecture : on doit la donner a right
		close(pipefd[1]);
	}
	right_pid = fork();
	if (!right_pid)
	{
		// error
	}
	if (right_pid == 0)
	{
		if ((*ast)->right->token->token == PIPE)
		{
			dup2((*pipes)->fd[0], STDIN_FILENO);
			close((*pipes)->fd[0]);
		}
		/* dprintf(2, "RIGHT read in %d\n", (*pipes)->fd[0]); */
		return (exec_ast(&right_branch, env, pipes));
	}
	else
	{
		/* close(pipefd[0]); */
		free_pipes(pipes);
		/* free(pipes); */
		pipes = NULL;
		exit_code = wait_children(right_pid, left_pid);
		/* free_tree(*ast); */
		return(exit_code);
	}
	return (1);
}
// normalement on ne devrait pas arriver au dernier return !

//BUILT_IN : PARENT
//CMD : CHILD

int error_cmd_not_found(char *cmd)
{
	char *s;
	char *tmp;

	s = ft_strjoin("mimishell: ", cmd);	
	tmp = s;
	s = ft_strjoin(s, ": command not found\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (127);
}

//a tester !!
int error_cmd_perm_denied(char *cmd)
{
	char *s;
	char *tmp;

	s = ft_strjoin("mimishell: ", cmd);	
	tmp = s;
	s = ft_strjoin(s, ": Permission denied\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (126);
}

int	exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes)
{
	char	**strings_env;
	pid_t	pid;
	int exit_code;

	if ((*ast)->token->token == BUILT_IN)
	{
		exit_code = builtins((*ast)->token->content, env, ast, pipes);
		return (exit_code);
	}

	if ((*ast)->token->token == CMD)
	{
		pid = fork();
		if (pid == -1)
		{
			// gerer l'erreur
		}
		if (pid == 0)
		{
			strings_env = lst_to_array(env);
			execve((*ast)->token->content[0], (*ast)->token->content,
				strings_env);
			free_array(strings_env);
			/* free_tree(*ast); */
			free_list(env);
			exit(1);
		}
		else
		{
			exit_code = wait_children(pid, pid);
			//le faire avant !
			free_pipes(pipes);
			pipes = NULL;
			/* free_tree(*ast); */
			free_list(env);
			return (exit_code);
		}
	}
	return (1); //on ne devrait pas arriver ici
}

int	boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes)
{
	int		exit_code;
	t_tree	*tmp;

	exit_code = 0;
	/* if (find_expands(ast->left)) */
	/* 	expand_variables(ast->left); */
	tmp = (*ast)->left;
	exit_code = exec_ast(&tmp, env, pipes);
	if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0
			&& (*ast)->token->token == O_OR))
	{
		/* if (find_expands(ast->right)) */
		/* 	expand_variables(ast->right); */
		tmp = (*ast)->right;
		return (exec_ast(&tmp, env, pipes));
		return (1);
	}
	else
		return (exit_code);
}

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

int	redirect_stdio(t_tree **ast, t_var **env, t_pipe **pipes)
{
	t_tree	*left;
	t_tree	*right;

	left = (*ast)->left;
	right = (*ast)->right;
	if (open_dup2_close(ast, (*ast)->token->token))
	{
		// Error
	}
	if (right)
		exec_ast(&right, env, pipes);
	if (!left)
		return (0);
	return (exec_ast(&left, env, pipes));
}

int	exec_ast(t_tree **ast, t_var **env, t_pipe **pipes)
{

	if ((*ast)->token->error == 127)
		return (error_cmd_not_found((*ast)->token->content[0]));
	if ((*ast)->token->error == 126)
		return (error_cmd_perm_denied((*ast)->token->content[0]));
	//O_AND VONT VIRER 
	/* if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR) */
	/* 	return (boolean_operators(ast, env, pipes, pids)); */
	/* if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND */
	/* 	|| (*ast)->token->token == TRUNC)  */
		/* redirect_stdio(ast, env, pipes, pids); */
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, pipes));
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)
		return (exec_cmd(ast, env, pipes));
		//un token VAR ?
	//Ultrabonus
		//un token Alias
	  //un token shell_func
	  //un token substitution cmd ?
	return (0);
}

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

/* int find_expands(t_tree *ast) */
/* { */
/* 	// */
/* 	return (0); */
/* } */
/**/
/* int expand_variables(t_tree **ast) */
/* { */
/* 	// */
/* 	return (0); */
/* } */

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
