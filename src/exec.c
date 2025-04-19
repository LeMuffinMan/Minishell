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

int	wait_children(pid_t last_child, t_pids **pids)
{
	t_pids *tmp;
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	if (pids)
		tmp = *pids;
	else
		tmp = NULL;
	while(tmp && tmp->next)
	{
		waitpid(tmp->pid, &status, 0);
		tmp = tmp->next;
	}
	waitpid(last_child, &status, 0);
	//ici on free la liste des pids
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

int	builtins(char **arg, t_var **env, t_tree **ast, t_pipe **pipes)
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
		return (builtin_exit(arg, env, ast, pipes));
	else
		return (1);
}

int free_lists_and_exit(t_var **env, t_tree **ast, t_pipe **pipes)
{
	(void)env;
	(void)pipes;
	(void)ast;
	//free_env
	//free_ast
	//free_pipes
	return (0);
}

/* int get_pipe(int fd[2], t_pipe **pipes) */
/* { */
/* 	t_pipe *new_pipe; */
/* 	t_pipe *last; */
/**/
/* 	new_pipe = malloc(sizeof(t_pipe)); */
/* 	if(!new_pipe) */
/* 	{ */
/* 		//malloc error */
/* 	} */
/* 	if (!*pipes) */
/* 		*pipes = new_pipe; */
/* 	else  */
/* 	{ */
/*     last = *pipes; */
/*     while (last->next) */
/*         last = last->next; */
/*     last->next = new_pipe; */
/* 	} */
/* 	new_pipe->next = NULL; */
/* 	if (pipe(fd) == -1) */
/* 	{ */
/* 		//error de pipe */
/* 	} */
/* 	new_pipe->fd[0] = fd[0]; */
/* 	new_pipe->fd[1] = fd[1]; */
/* 	return (0); */
/* } */

int add_pid(pid_t new_pid, t_pids **pids)
{
	t_pids *new_node; 
	t_pids *last;

	last = NULL;
	new_node = malloc(sizeof(t_pids));
	if (!new_node)
	{
		//malloc error
	}
	if (!*pids)
		*pids = new_node;
	else
	{
		last = *pids;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
	new_node->next = NULL;
	new_node->pid = new_pid;
	return (0);
}

int print_pipes(t_pipe **pipes)
{
	t_pipe *tmp;
	int i;

	i = 1;
	tmp = *pipes;
	while (tmp)
	{
		/* dprintf(2, "pipe #%d : fd[0] = %d | fd[1] = %d\n", i, (*pipes)->fd[0], (*pipes)->fd[1]); */
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int add_pipe(int fd[2], t_pipe **pipes)
{
	t_pipe *new_pipe;

	if (pipe(fd) == -1)
	{
		//error
	}
	new_pipe = malloc(sizeof(t_pipe));
	if (!new_pipe)
	{
		//error
	}
	new_pipe->next = *pipes;
	new_pipe->prev = NULL;
	new_pipe->fd[0] = fd[0];
	new_pipe->fd[1] = fd[1];
	if (*pipes)
		(*pipes)->prev = new_pipe;
	*pipes = new_pipe; 
	/* print_pipes(pipes); */
	return (0);
}

int free_pids(t_pids **pids)
{
	t_pids *tmp;

	while(*pids && (*pids)->next)
	{
		tmp = *pids;
		*pids = (*pids)->next;
		free(tmp);
	}
	return (0);
}

int free_pipes(t_pipe **pipes)
{
	t_pipe *tmp;

	tmp = *pipes;
	while (*pipes && (*pipes)->next)
	{
		tmp = *pipes;
		*pipes = (*pipes)->next;
		free(tmp);
	}
	free(*pipes);
	pipes = NULL;
	return (0);
}

int exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
{
	pid_t left;
	pid_t right;
	t_tree *left_branch;
	t_tree *right_branch;
	t_pipe *tmp;
	int pipefd[2];

	right_branch = (*ast)->right;
	left_branch = (*ast)->left;
	//add_pipe : pipe le fd fourni ET l'ajoute a la liste
	add_pipe(pipefd, pipes); //on ajoute un pipe au sommet
	//dans tous les cas on lance a gauche
	left = fork();	
	if (!left)
	{
		//error
	}
	if (left == 0)
	{
		//le child n'a pas besoin d'attendre les pids donc on free la liste 
		free_pids(pids);
		//Dans tous les cas, on veut ecrire dans le pipe ici
		dup2((*pipes)->fd[1], STDOUT_FILENO);
		close((*pipes)->fd[1]);
		/* dprintf(2, "write_fd duped : %d\n", pipefd[1]); */
		close(pipefd[1]);
		//Si pipes->next est NULL : c'est qu'on est le premier pipe : donc pas besoin de dup la lecture 
		if (!(*pipes)->next)
		{
			/* tmp = (*pipes)->next; */
			/* (*pipes)->next = NULL; */
			dup2(pipefd[0], STDIN_FILENO);
			/* dprintf(2, "pipes->fd[0] duped : %d\n", pipefd[0]); */
			close(pipefd[0]);
			/* free(tmp); */
		}
		else //sinon, c'est qu'on doit brancher la lecture du pipe precedent
		{
			dup2((*pipes)->next->fd[0], STDIN_FILENO);
			close((*pipes)->next->fd[0]);
			//la cmd gacuhe ne lira pas dans le pipe qu'on vient de creer : soit elle lit dans STDIN, soit dans le pipe precedent
			close(pipefd[0]);
		}
		return (exec_ast(&left_branch, env, pipes, pids));
	}
	else //left parent
	{
		//si on a un pipe->next, ca veut dire qu'on est DANS un pipeline : le child a eu la lecture du pipe precedent 
		//donc on ferme la lecture et on free le pipe precedent
		if (pipes && (*pipes)->next)
		{
			tmp = (*pipes)->next;
			(*pipes)->next = NULL;
			close(tmp->fd[0]);
			free(tmp);
			//pour le parent on free le dernier pipe qu'on avait garder pour brancher la lecture de right
		}
		//on ajoute un pid a la liste pour les attendre a la fin 
		add_pid(left, pids);
		//si y'a pas de next : le pipe de la liste est celui qu'on vient de creer, on veut le garder pour la cmd qui suit
		//donc on ferme pour le parent, mais on ne free pas 
		/* close(pipefd[0]); */
		close(pipefd[1]);
	}
	//cas d'un pipeline : on lance la 1ere commande mais on rappelle exec_ast pour executer le pipe qui suit 
	if (right_branch && right_branch->token->token == PIPE)
		return(exec_ast(&right_branch, env, pipes, pids));
	//cas de la fin d'un pipeline / d'un pipe tout seul : on lance les deux
	right = fork();
	if (!right)
	{
		//error
	}
	//la derniere commande executee lit dans le pipe qu'on vient de creer, pas besoin de la liste 
	if (right == 0)
	{
		//le child n'a pas besoin d'attendre les pids donc on free la liste 
		free_pids(pids);
		//la cmd droite doit ecrire dans la STDOUT donc on close l'entree du pipe
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		/* dprintf(2, "read_fd duped : %d\n", pipefd[0]); */
		close(pipefd[0]);
		return (exec_ast(&right_branch, env, pipes, pids));
	}
	else
	{
		print_pipes(pipes);
		/* close((*pipes)->next->fd[0]); */
		close((*pipes)->fd[0]);
		close((*pipes)->fd[1]);
		/* print_pipes(pipes); */
		return(wait_children(right, pids));
	}
	//ON NE DEVRAIT JAMAIS ARRIVER ICI !
	return (1);
}

int exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes)
{
	char **strings_env;
	pid_t pid;

	/* dprintf(2, "exec_cmd begins : cmd = %s\n", (*ast)->token->content[0]); */
	//le parent execute que si c'est un builtin sans pipe : !*pipes == si la liste est vide
	if ((*ast)->token->token == BUILT_IN && !*pipes)
		return (builtins((*ast)->token->content, env, ast, pipes));
	//dans tous les autres cas c'est un child
	//cas d'un built in a executer dans un child : l'appel de cette fct vient deja d'un fork d'un pipe
	if ((*ast)->token->token == BUILT_IN)
		exit(builtins((*ast)->token->content, env, ast, pipes));
	//Ici, on ne peut QUE avoir des CMD 
	if (!pipes) //cas d'une cmd a executer alors qu'il ne vient pas d'un pipe : il faut fork
	{
		/* dprintf(2, "ici\n"); */
		pid = fork();	
		if (pid == -1)
		{
			//gerer l'erreur
		}
		if (pid == 0)
		{
			strings_env = lst_to_array(env);
			/* printf("NO PIPE : exec cmd : %s | fd[0] : %d / fd[1] = %d\n", (*ast)->token->content[0], STDIN_FILENO, STDOUT_FILENO); */
			execve((*ast)->token->content[0], (*ast)->token->content, strings_env);
			// si on passe ici, c'est que l'execve a echouer
			free_array(strings_env);
			exit(1); //exit parce qu'on est dans un child 
		}
		else // on wait juste vu qu'on n'a pas de pipe ?
		{
			free_pipes(pipes);
			pipes = NULL;
			return (wait_children(pid, NULL));
		}
	}
	else  //Si on a une cmd dans un pipe : on a deja fork
	{
		strings_env = lst_to_array(env);
		/* dprintf(2, "PIPED : exec cmd : %s | fd[0] = %d / fd[1] = %d\n", (*ast)->token->content[0], STDIN_FILENO, STDOUT_FILENO); */
		execve((*ast)->token->content[0], (*ast)->token->content, strings_env);
		// si on passe ici, c'est que l'execve a echouer
		free_array(strings_env);
		exit(1); //exit parce qu'on est dans un child 
	}
	return (1);
}

int boolean_operators(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
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
	exit_code = exec_ast(&tmp, env, pipes, pids);
	if ((exit_code == 0 && (*ast)->token->token == O_AND) || (exit_code != 0 && (*ast)->token->token == O_OR))
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

int exec_ast(t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids)
{
	//1 boolean_operators
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (boolean_operators(ast, env, pipes, pids));
	//3 Redirections : on open dup close au fur et a mesure en descendant
	/* if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND || (*ast)->token->token == TRUNC) */
	/* 	return (redirect_stdio(ast, env, pipes)); */
	//4 on pipe les nodes et on les lances 
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, env, pipes, pids));
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

