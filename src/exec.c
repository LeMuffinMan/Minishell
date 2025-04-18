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


int	wait_children(t_pids **pids)
{
	t_pids *tmp;
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	while(pids->next)
	{
		waitpid(pids->pid, &status, 0);
		pids = pids->next;
	}
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

int get_pipe(int fd[2], t_pipe **pipes)
{
	t_pipe *new_pipe;
	t_pipe *last;

	new_pipe = malloc(sizeof(t_pipe));
	if(!new_pipe)
	{
		//malloc error
	}
	if (!*pipes)
		*pipes = new_pipe;
	else 
	{
    last = *pipes;
    while (last->next)
        last = last->next;
    last->next = new_pipe;
	}
	new_pipe->next = NULL;
	if (pipe(new_pipe->fd) == -1)
	{
		//error de pipe
	}
	return (0);
}

int setup_pipeline(t_tree **ast, t_var **env, t_pipes **pipes)
{
	t_pipe *new_pipe;

	if (get_pipe(pipes))
	{
		//le pipe a foire !
	}
	new_pipe = *pipes;
	while(new_pipe->next)
		new_pipe = new_pipe->next;
	// on branche a gauche
	(*ast)->left->token->fd[1] = new_pipe->fd[1];
	printf("new_pipe->fd[1] = %d | node->fd[1] = %d\n", new_pipe->fd[1], (*ast)->left->token->fd[1]);
	//selon si on est a la fin du pipeline ou non, on branch a droite ou on fait la jonction
	if ((*ast)->right->token->token == PIPE))
	{
		setup_pipeline((*ast)->right, env, pipes);
		(*ast)->right->left->token->fd[0] = new_pipe->fd[0];
	}
	(*ast)->left->token->fd[0] = new_pipe->fd[0];
	return (0);
}

int start_pipeline(t_var **env, t_tree **ast, t_pipe **pipes, t_pid **pids)
{
	if (ast->right && ast->right->token->token == PIPE)
	{
		exec_cmd(ast->left, pids);
		exec_cmd(ast->right->left, pids);
		return (start_pipeline(env, ast->right, pipes, pids));
	}
	else
	{
		exec_cmd(ast->right, pids);
		return (wait_children(pids));
	}
}

typedef struct s_pids
{
	pid_t pid;
	struct s_pids *next;
} t_pids;

int exec_pipe(t_tree **ast, t_var **env, t_pipe **pipes)
{
	t_pids *pids;

	pids = NULL;
	if (setup_pipeline(ast, env, pipes))
		return (free_lists_and_exit(env, ast, pipes));
	if (start_pipeline(ast, env, pipes, &pids));
		return (free_lists_and_exit(env, ast, pipes));
	return(wait_children(&pids));
}

int close_all_pipes(t_pipe **pipes)
{
  t_pipe *tmp;

  tmp = *pipes;
  while (tmp)
  {
    if (tmp->fd[0] > 2)
        close(tmp->fd[0]); //proteger
    if (tmp->fd[1] > 2)
        close(tmp->fd[1]);
    tmp = tmp->next;
  }
  return (0);
}

int dup_close_pipe(t_tree **ast, t_pipes **pipes)
{
	t_pipes *tmp;
	//si on est dans le pipeline
	if ((*ast)->token->fd[0] > 2 && (*ast)->token->fd[1] > 2)
	{
		dup2((*ast)->token->fd[0], STDIN_FILENO);
		close((*ast)->token->fd[0]);
		dup2((*ast)->token->fd[1], STDIN_FILENO);
		close((*ast)->token->fd[1]);
		tmp = *pipes->next;
		free(pipes);
		pipes = tmp;
	}
	// si on est a la fin du pipeline
	if ((*ast)->token->fd[0] > 2 && (*ast)->token->fd[1] < 0)
	{
		dup2((*ast)->token->fd[0], STDIN_FILENO);
		close((*ast)->token->fd[0]);
		close(pipes->fd[1]);
		free(pipes);
	}
	// si on est au debut d'un pipeline
	if ((*ast)->token->fd[1] > 2 && (*ast)->token->fd[0] < 0)
	{
		dup2((*ast)->token->fd[1], STDOUT_FILENO);
		close((*ast)->token->fd[1]);
		close(pipes->fd[0]);
		tmp = *pipes->next;
		free(pipes);
		*pipes = tmp;
	}
	return (0);
}


int add_pid(pid_t pid, t_pids **pids)
{
	t_pids *new_pid;
	t_pids *last;

	new_pid = malloc(sizeof(t_pids));
	if (!new_pid)
	{
		//malloc error
	}
	if (!*pids)
		*pids = new_pid;
	else 
	{
    last = *pipes;
    while (last->next)
        last = last->next;
    last->next = new_pid;
	}
	new_pid->next = NULL;
	new_pid->pid = pid;
	return (0);
}

int exec_cmd(t_tree **ast, t_var **env, t_pipe **pipes, t_pid **pids)
{
	char **translated_env;
	pid_t pid;
	t_pipe *tmp;

	//le parent execute que si c'est un builtin sans pipe
	if ((*ast)->token->token == BUILT_IN && ((*ast)->token->fd[1] == -1 && (*ast)->token->fd[0] == -1))
		return (builtins((*ast)->token->content, env));
	//dans tous les autres cas c'est un child
	pid = fork();
	if (pid < 0)
		return (free_lists_and_exit(env, ast, pipes));
	add_pid(pid, pids);
	if (pid == 0)
	{
		//on dup et close ce qu'il faut pour ce child
		dup_close_pipe(pipes);
		//on ferme et on free ? toute la liste des pipes qui suivent
		if (pipes->next);
			close_all_pipes(pipes->next);
		//si next est null, on a free dans dup_close
		printf("env should write in fd 4 : %d\n", (*ast)->token->fd[1]);
 		if ((*ast)->token->token == BUILT_IN)
			exit(builtins((*ast)->token->content, env));
		else
		{
			translated_env = lst_to_array(env);
			printf("exec cmd : %s\n", (*ast)->token->content[0]);
			execve((*ast)->token->content[0], (*ast)->token->content, translated_env);
		}
	}
	else
	{
		//on ferme et free le morceau de pipe qu'on vient d'envoyer
		close((*pipes)->fd[0]);
		close((*pipes)->fd[1]);
		tmp = (*pipes)->next;
		free(pipes);
		pipes = tmp;
		return(0);
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

