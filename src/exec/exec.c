/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:36:34 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"
#include "env_utils.h"
#include "utils.h"
#include "libft.h"
#include "list.h"
#include "signals.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h> // compiler avec -l readline

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
	else if (!ft_strncmp(arg[0], "source", 7))
		return (builtin_source((*ast)->right->token->content[0], env));
	else
		return (1);
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
		close_origin_fds(origin_fds);
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
			close_origin_fds(origin_fds);
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

void handle_child_sigint(int sig)
{
    (void)sig;
    /* write(STDOUT_FILENO, "\n", 1);  // Force un saut de ligne */
    exit(130);  // Code de sortie standard pour SIGINT (128 + 2)
}

//a bouger !
void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_child_sigint; 
	// SIGINT par défaut dans les enfants
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	// SIGQUIT par défaut dans les enfants
	sa.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &sa, NULL);
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
	if ((*ast)->token->token == BUILT_IN  || !ft_strncmp((*ast)->token->content[0], "source", 7))
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
    	setup_child_signals(); // a tester !
			close_origin_fds(origin_fds);
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
			set_signals(1);
			exit_code = wait_children(pid, pid);
			update_env(env);
			update_last_arg_var(env, (*ast)->token->content);
      set_signals(0); // a virer ?
      rl_on_new_line();
			return (exit_code);
		}
	}
	return (1); //on ne devrait pas arriver ici
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
	if ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD  || !ft_strncmp((*ast)->token->content[0], "source", 7))
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

//modifs pour le ctrl dans un pipe :
//
//il faut avoir un check is_in_pipe dans cmd : pour gerer un cat tout seul
//il faut deplacer les ajouts de set sig etc faites dans cmd dans exec_pipe
//il faut modifier la fonction wait_children pour qu'il recupere correctement le signal
//https://chat.deepseek.com/a/chat/s/a6fb8416-77db-418a-9c33-91607fa40c13

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

