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
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_pipes
{
	int fd[2];
	s_pipes *next;
} t_pipes;

typedef struct s_pids
{
	t_pid pid;
	s_pids *next;
} t_pids;

int	builtins(char **arg, t_var **env)
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
		return (builtin_exit(arg, env));
	else
		return (1);
}

int get_fds_for_pipe(t_ast *ast)
{
	t_ast *tmp;
	int fd[2];

	//on cherche a gauche le node le plus a droite
	tmp = ast->left;
	while (tmp->right)
		tmp = tmp->right;
	fd[1] = tmp->token->fd[1];
	// stocker les fd utilises dans une liste ? ou juste fermer s'ils sont differents de -1 a la fin de l'exec ?
	//on cherche a droite le node le plus a gauche
	tmp = ast->right;
	while (tmp->left)
		tmp = tmp->left;
	fd[0] = tmp->token->fd[0];
	return (fd);
}

int add_pipe(int fd[2], t_pipe **pipes)
{
	t_pipes *node;
	t_pipes *tmp;

	if (pipe(fd) != 0)
		return (1); //des codes d'erreurs specifiques ?
	node = malloc(sizeof(t_pipe));
	if (!node)
		return (1)
	node->next = NULL;
	node->fd = fd;
	if (!*pipes)
		*pipes = node;
	else
	{
		tmp = *pipes;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = node;		
	}
	return(0);
}

int free_lists_and_exit(t_var **env, t_ast **ast, t_pipes **pipes)
{
	//free_env
	//free_ast
	//free_pipes
	return (1);
}

int exec_pipe(t_ast **ast, t_var **env, t_pipes **pipes)
{
	//add_pipe recoit les deux fd a piper, les pipes, et ajoute un node a la liste des pipes
	if (add_pipe(get_fds_for_pipe(ast), pipes))
		return (free_lists_and_exit(env, ast, pipes));
	//ici on est ok pour lancer la commande a gauche
	exec_ast(ast->left, env);
	//on rapporte l'exit code de la commande droite
	return (exec_ast(ast->right, env));
}

int exec_builtins(t_ast **ast, t_var **env, t_pipes **pipes)
{
	t_pid pid;
	//aucun pipe branche : le parent execute
	if (ast->token->fd[1] < 0 && ast->token->fd[0] < 0)
		return (builtins(ast->token->content, env));
	//au moins un pipe bracnhe : le child execute
	else
	{
		pid = fork();
		if (pid < 0)
			return (free_lists_and_exit(env, ast, pipes));
		if (pid == 0)
			return (builtins(ast->token->content, env));
		else 
		{
			//close des trucs ?
			return (wait_children());
		}
	}
}

int exec_cmd(t_ast **ast, t_var **env, t_pipes **pipes)
{
	t_pid pid;

	pid = fork();
	if (pid < 0)
		return (free_lists_and_exit(env, ast, pipes));
	if (pid == 0)
	{
		translated_env = lst_to_array(env);
		execve(ast->token->content[0], ast->token->content, translated_env);
	}
	else 
	{
		//close des trucs ?
		return (wait_children());
	}
}

int redirect_stdio(t_ast **ast, t_var **env, t_pipes **pipes)
{
	if (ast->token->token == APPEND || ast->token->token == TRUNC)
	{
		fd[1] = get_fd_to_dup(ast->left, env);
		open_and_dup(ast);
		return (exec_ast(ast->left, env));
	}
	if (ast->token->token == R_IN)
	{
		fd[0] = get_fd_to_dup(ast->right, env);
		open_and_dup(ast);
		return (exec_ast(ast->left, env));
	}
}

int exec_and(t_ast **ast, t_var **env, t_pipes **pipes)
{
	exit_code = exec_ast(ast->left, env);
	if (exit_code == 0)
		return (exec_ast(ast->right, env));
}

int exec_or(t_ast **ast, t_var **env, t_pipes **pipes)
{
	exit_code = exec_ast(ast->left, env);
	if (exit_code != 0)
		return (exec_ast(ast->right, env));
}

int find_here_docs(t_ast **ast)
{
	if (ast->token->token == HD)
		return (1);
	else if (ast->left)
		return (get_here_docs(ast->left));
	else if (ast->right)
		return (get_here_docs(ast->right));
	return (0);
}

int find_here_docs(t_ast *ast)
{
	//
	return (0);
}


int find_expands(t_ast *ast)
{
	//
	return (0);
}

int exec_here_docs(t_ast **ast)
{
	//
	return (0);
}

int expand_variables(t_ast **ast)
{
	//
	return (0);
}

int exec_ast(t_ast **ast, t_var **env, t_pipes **pipes)
{
	int exit_code;

	//1 ouvrir TOUS les here_docs et les remplir avec leur lim
	if (find_here_docs(ast))
		exec_here_docs(ast);
	//2 Quelle(s) sequence(s) ?
	if (ast->token->token == O_AND)
		return (exec_and(ast, env, pipes));
	else if (ast->token->token == O_OR) 
		return (exec_or(ast, env, pipes));
	//3 Expands ?
	if (find_expands(ast))
		expand_variables(ast);
	//4 Redirections : on open dup close au fur et a mesure en descendant
	if (ast->token->token == R_IN || ast->token->token == APPEND || ast->token->token == TRUNC)
		return (redirect_stdio(ast, env, pipes));
	//5 on pipe les nodes et on les lances 
	if (ast->token->token == PIPE)
		return (exec_pipe(ast, env, pipes));
	//6 CMD :
	if (ast->token->token == BUILT_IN)
		return (exec_builtins(ast, env, pipes));
	if (ast->token->token == CMD)
		return (exec_cmd(ast, env, pipes));
	return (0);
}


