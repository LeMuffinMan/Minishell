/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/17 15:22:34 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int	wait_children(pid_t pid)
{
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	if (exit_code == EXIT_SUCCESS && WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (exit_code == EXIT_SUCCESS && WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

int	builtins(char **arg, t_var **env)
{
	/* printf("exec builtin %s\n", arg[0]); */
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
		return (builtin_exit(arg, env));
	else
		return (1);
}

//stocker les fd dans une liste pour pouvoir les fermer proprement ? 
int get_read_end(t_tree **ast)
{
	t_tree *tmp;

	tmp = (*ast)->right;
	while (tmp->left)
		tmp = tmp->left;
	return (tmp->token->fd[0]);
}

int get_write_end(t_tree **ast)
{
	t_tree *tmp;

	tmp = (*ast)->left;
	while (tmp->right)
		tmp = tmp->right;
	return (tmp->token->fd[1]);
}

int add_pipe(t_tree **ast, t_pipe **pipes)
{
	int fd[2];
	/* t_tree *tmp; */

	(void)pipes;
	if (pipe(fd) == -1)
	{
		//error de pipe
	}
	(*ast)->left->token->fd[1] = fd[1];
	printf("fd[1] = %d | node->fd[1] = %d\n", fd[1], (*ast)->left->token->fd[1]);
	if ((*ast)->right->token->token != PIPE)
	{
		(*ast)->right->token->fd[0] = fd[0];
		printf("fd[0] = %d | node->fd[0] = %d\n", fd[0], (*ast)->right->token->fd[0]);
	}
	else 
	{
		/* (*ast)->right->left->token->fd[0] = fd[0]; */
		/* printf("fd[0] = %d | node->fd[0] = %d\n", fd[0], (*ast)->right->left->token->fd[0]); */
	}
	//asumming write node is always 1st left && read node is either 1st right or 2nd right+left
	return (0);
}

int free_pipes(t_pipe **pipes)
{
	t_pipe *tmp;
	t_pipe *last;

	tmp = *pipes;
	while (tmp)
	{
		//close
		last = tmp;
		tmp = tmp->next;
		free(last);
	}
	return (0);
}

int free_lists_and_exit(t_var **env, t_tree **ast, t_pipe **pipes)
{
	(void)env;
	(void)pipes;
	(void)ast;
	//free_env
	//free_ast
	//free_pipes
	return (1);
}

int exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes)
{
	t_tree *tmp;
	int exit_code;

	exit_code = 0;
	//add_pipe recoit les deux fd a piper, les set_pipe, et ajoute un node a la liste des pipes
	if (add_pipe(ast, pipes))
		return (free_lists_and_exit(env, ast, pipes));
	//ici on est ok pour lancer la commande a gauche
	tmp = (*ast)->left;
	/* if (dup2(tmp->token->fd[1], STDOUT_FILENO) == -1) */
	/* { */
	/* 	//gerer l'erreur */
	/* } */
	/* printf("tmp->token->fd[1] = %d\n", tmp->token->fd[1]); */
	exec_ast(&tmp, env, pipes);
	close()
	//on rapporte l'exit code de la commande droite
	tmp = (*ast)->right;
	/* if (dup2(tmp->token->fd[0], STDIN_FILENO) == -1) */
	/* { */
	/* 	//gerer l'erreur */
	/* } */
	/* printf("tmp->token->fd[0] = %d\n", tmp->token->fd[0]); */
	exit_code = exec_ast(&tmp, env, pipes);
	return (exit_code);
}

int exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes)
{
	pid_t pid;
	char **translated_env;
	//un built in avec aucun pipe branche : le parent execute
	if ((*ast)->token->token == BUILT_IN && ((*ast)->token->fd[1] > 2 || (*ast)->token->fd[0] > 2))
	{
		printf("builtin piped about to execute : %s fd[0] = %d | fd[1] = %d\n", (*ast)->token->content[0], (*ast)->token->fd[0], (*ast)->token->fd[1]);
		if((*ast)->token->fd[0] > 2)
		{
			if (dup2((*ast)->token->fd[0], STDIN_FILENO) == -1)
			{
				//gerer l'erreur
			}
			close((*ast)->token->fd[0]);
		}
		if((*ast)->token->fd[1] > 2)
		{
			if (dup2((*ast)->token->fd[1], STDOUT_FILENO) == -1)
			{
				//gerer l'erreur
			}
			close((*ast)->token->fd[1]);
		}
		printf("exec builtin : %s\n", (*ast)->token->content[0]);
		return (builtins((*ast)->token->content, env));
	}
	else
	{
		printf("ici ? \n");
		//au moins un pipe bracnhe : le child execute
		pid = fork();
		if (pid < 0)
			return (free_lists_and_exit(env, ast, pipes));
		if (pid == 0 && (*ast)->token->token == BUILT_IN)
			return (builtins((*ast)->token->content, env));
		else if (pid == 0 && (*ast)->token->token == CMD)
		{
			if((*ast)->token->fd[0] > 2)
			{
				if (dup2((*ast)->token->fd[0], STDIN_FILENO) == -1)
				{
					//gerer l'erreur
				}
				close((*ast)->token->fd[0]);
			}
			if((*ast)->token->fd[1] > 2)
			{
				if (dup2((*ast)->token->fd[1], STDOUT_FILENO) == -1)
				{
					//gerer l'erreur
				}
				close((*ast)->token->fd[1]);
			}
			translated_env = lst_to_array(env);
			printf("exec cmd : %s\n", (*ast)->token->content[0]);
			execve((*ast)->token->content[0], (*ast)->token->content, translated_env);
		}
		else 
		{
			if ((*ast)->token->fd[0] > 2)
				close((*ast)->token->fd[0]);
			if ((*ast)->token->fd[1] > 2)
				close((*ast)->token->fd[1]);
			//close des trucs ?
			return (wait_children(pid));
		}
		return (1); // on ne devrait JAMAIS tomber sur ce return 
	}
}

/* int redirect_stdio(t_tree **ast, t_var **env, t_pipe **pipes) */
/* { */
/* 	if ((*ast)->token->token == APPEND || (*ast)->token->token == TRUNC) */
/* 	{ */
/* 		fd[1] = get_fd_to_dup(*ast)->left, env); */
/* 		open_and_dup(ast); */
/* 		return (exec_ast(*ast)->left, env)); */
/* 	} */
/* 	if ((*ast)->token->token == R_IN) */
/* 	{ */
/* 		fd[0] = get_fd_to_dup(*ast)->right, env); */
/* 		open_and_dup(ast); */
/* 		return (exec_ast(*ast)->left, env)); */
/* 	} */
/* } */

int boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes)
{
	int exit_code;
	t_tree *tmp;

	exit_code = 0;
	(void)tmp;
	(void)pipes;
	(void)env;
	/* if (find_expands(ast->left)) */
	/* 	expand_variables(ast->left); */
	tmp = (*ast)->left;
	exit_code = exec_ast(&tmp, env, pipes);
	if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0 && (*ast)->token->token == O_OR))
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

char *enum_to_string(t_type enumValue)
{
	char *s;

  if (enumValue == CMD)
      s = ft_strdup("CMD");
  else if (enumValue == BUILT_IN)
      s = ft_strdup("BUILT_IN");
  else if (enumValue == APPEND)
      s = ft_strdup("APPEND");
  else if (enumValue == D_QUOTE)
      s = ft_strdup("D_QUOTE");
  else if (enumValue == HD)
      s = ft_strdup("HD");
  else if (enumValue == LIM)
      s = ft_strdup("LIM");
  else if (enumValue == O_AND)
      s = ft_strdup("O_AND");
  else if (enumValue == O_OR)
      s = ft_strdup("O_OR");
  else if (enumValue == PIPE)
      s = ft_strdup("PIPE");
  else if (enumValue == R_IN)
      s = ft_strdup("R_IN");
  else if (enumValue == S_QUOTE)
      s = ft_strdup("S_QUOTE");
  else if (enumValue == TRUNC)
      s = ft_strdup("TRUNC");
  else if (enumValue == WILDCARD)
      s = ft_strdup("WILDCARD");
  else
      s = ft_strdup("UNKNOWN");
  return (s);
}

int exec_ast(t_tree **ast, t_var **env, t_pipe **pipes)
{
	//1 boolean_operators
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (boolean_operators(ast, env, pipes));
	//3 Redirections : on open dup close au fur et a mesure en descendant
	/* if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND || (*ast)->token->token == TRUNC) */
	/* 	return (redirect_stdio(ast, env, pipes)); */
	//4 on pipe les nodes et on les lances 
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, pipes));
 	/* char *s = enum_to_string((*ast)->token->token); */
	/* printf("token = %s\n", s); */
	/* free(s); */
	//5 on execute la commande :
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)
		return (exec_cmd(ast, env, pipes));
	return (0);
}

//exec_ast.c
/* int exec_ast(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int redirect_stdio(t_tree **ast, t_var **env, t_pipe **pipes); */
/* int exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes); */

//pipe_utils.c
/* int get_fds_for_pipe(t_tree *ast); */
/* int add_pipe(int fd[2], t_pipe **pipes); */
/* int free_pipes(t_pipe **pipes); */

//exec_utils.c
/* int expand_variables(t_tree **ast); */
/* int find_expands(t_tree *ast) */
/* int free_lists_and_exit(t_var **env, t_tree **ast, t_pipe **pipes) */

