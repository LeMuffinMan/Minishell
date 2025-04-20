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

// checker les fd open : des cas ou ca peut arriver je pense
int	exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
{
	pid_t	left;
	pid_t	right;
	t_tree	*left_branch;
	t_tree	*right_branch;
	t_pipe	*tmp;
	int		pipefd[2];

	right_branch = (*ast)->right;
	left_branch = (*ast)->left;
	add_pipe(pipefd, pipes);
	left = fork();
	if (!left)
	{
		// error
	}
	if (left == 0)
	{
		free_pids(pids);
		dup2((*pipes)->fd[1], STDOUT_FILENO);
		close((*pipes)->fd[1]);
		close(pipefd[1]);
		if (!(*pipes)->next)
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
		}
		else
		{
			dup2((*pipes)->next->fd[0], STDIN_FILENO);
			close((*pipes)->next->fd[0]);
			close(pipefd[0]);
		}
		return (exec_ast(&left_branch, env, pipes, pids));
	}
	else
	{
		if (pipes && (*pipes)->next)
		{
			tmp = (*pipes)->next;
			(*pipes)->next = NULL;
			close(tmp->fd[0]);
			free(tmp);
		}
		add_pid(left, pids);
		close(pipefd[1]);
	}
	if (right_branch && right_branch->token->token == PIPE)
		return (exec_ast(&right_branch, env, pipes, pids));
	right = fork();
	if (!right)
	{
		// error
	}
	if (right == 0)
	{
		free_pids(pids);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		return (exec_ast(&right_branch, env, pipes, pids));
	}
	else
	{
		close((*pipes)->fd[0]);
		close((*pipes)->fd[1]);
		return (wait_children(right, pids));
	}
	return (1);
}
// normalement on ne devrait pas arriver au dernier return !

int	exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes)
{

	char	**strings_env;
	pid_t	pid;

	if ((*ast)->token->token == BUILT_IN && !*pipes)
		return (builtins((*ast)->token->content, env, ast, pipes));
	if ((*ast)->token->token == BUILT_IN)
		exit(builtins((*ast)->token->content, env, ast, pipes));
	if (!pipes)
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
			exit(1);
		}
		else
		{
			free_pipes(pipes);
			pipes = NULL;
			return (wait_children(pid, NULL));
		}
	}
	else
	{
		strings_env = lst_to_array(env);
		execve((*ast)->token->content[0], (*ast)->token->content, strings_env);
		free_array(strings_env);
		exit(1);
	}
	return (1);
}

int	boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
{
	int		exit_code;
	t_tree	*tmp;

	exit_code = 0;
	/* if (find_expands(ast->left)) */
	/* 	expand_variables(ast->left); */
	tmp = (*ast)->left;
	exit_code = exec_ast(&tmp, env, pipes, pids);
	if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0
			&& (*ast)->token->token == O_OR))
	{
		/* if (find_expands(ast->right)) */
		/* 	expand_variables(ast->right); */
		tmp = (*ast)->right;
		return (exec_ast(&tmp, env, pipes, pids));
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
		fd = open((*ast)->token->content[0], O_RDONLY);
		if (fd == -1)
		{
			// error
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (type == APPEND)
	{
		fd = open((*ast)->token->content[0], O_CREAT | O_WRONLY | O_APPEND,
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
		fd = open((*ast)->token->content[0], O_CREAT | O_WRONLY | O_TRUNC,
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

int	redirect_stdio(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
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
		if (exec_ast(&right, env, pipes, pids))
			return (1);
	return (exec_ast(&left, env, pipes, pids));
}

int	exec_ast(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
{
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (boolean_operators(ast, env, pipes, pids));
	/* if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC) */
	/* 	return (redirect_stdio(ast, env, pipes)); */
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, pipes, pids));
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)
		return (exec_cmd(ast, env, pipes));
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
