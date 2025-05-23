/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:49:28 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/18 16:12:35 by oelleaum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "aliases.h"
#include "builtins.h"
#include "env_utils.h"
#include "exec.h"
#include "libft.h"
#include "list.h"
#include "shell_fct.h"
#include "signals.h"
#include "utils.h"
#include <readline/readline.h> // compiler avec -l readline
#include <unistd.h>

//ULTRABONUS
int builtins_ultra_bonus(char **arg, t_lists *lists)
{
	if (!ft_strncmp(arg[0], "source", 7)) // mieux proteger ULTRA BONUS
		return (builtin_source((*lists->ast)->right->token->content[0], lists->env));
	/* else if (!ft_strncmp(arg[0], "history", 7)) // ULTRA BONUS */
	/* 	return (builtin_history((*ast)->right->token->content[0], env)); */
	return (1);
}

int	builtins(char **arg, t_lists *lists)
{
	if (!*arg)
		return (1);
	else if (!ft_strncmp(arg[0], "echo", 5))
		return (builtin_echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (builtin_cd(arg, lists->env));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (builtin_pwd(lists->env));
	else if (!ft_strncmp(arg[0], "export", 7))
		return (builtin_export(lists->env, arg));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (builtin_unset(lists->env, arg));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (builtin_env(lists->env, arg));
	else if (!ft_strncmp(arg[0], "exit", 5))
		return (builtin_exit(arg, lists->ast, lists));
	else
		return (builtins_ultra_bonus(arg, lists));
}

int setup_pipe_signals(struct sigaction *sa_ignore, struct sigaction *sa_orig)
{
	sigemptyset(&sa_ignore->sa_mask);
	sa_ignore->sa_handler = SIG_IGN;
	sa_ignore->sa_flags = 0;
	sigaction(SIGINT, sa_ignore, sa_orig); // On sauvegarde dans sa_orig
	return 0;
}

int left_child_execution(t_tree **ast, t_lists *lists)
{
	int exit_code;

	exit_code = 0;
	close_origin_fds(lists->origin_fds);
	close(lists->pipe_fd[0]);
	if ((*lists->pipes)->next)
	{
		dup2((*lists->pipes)->next->fd[0], STDIN_FILENO);
		close((*lists->pipes)->next->fd[0]);
		free((*lists->pipes)->next);
		(*lists->pipes)->next = NULL;
	}
	dup2(lists->pipe_fd[1], STDOUT_FILENO);
	close(lists->pipe_fd[1]);
	free_pipes(lists->pipes);
	exit_code = exec_ast(&(*ast)->left, lists);
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(exit_code);
	return (0);
}

int left_parent_execution(t_pipe **pipes, int pipefd[2])
{
	if ((*pipes)->next)
	{
		close((*pipes)->next->fd[0]);
		free((*pipes)->next);
		(*pipes)->next = NULL;
	}
	close(pipefd[1]);
	return (0);
}

int right_child_execution(t_tree **ast, t_lists *lists, int pipefd[2], t_pipe **pipes)
{
	int exit_code;

	exit_code = 0;
	close_origin_fds(lists->origin_fds);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	free_pipes(pipes);
	exit_code = exec_ast(&((*ast)->right), lists);
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(exit_code);
	return (0);
}

int exec_pipe_left_execution(t_tree **ast, t_lists *lists, pid_t pid)
{
	if (pid < 0)
	{
		// error
	}
	if (pid == 0)
	{
		if (left_child_execution(ast, lists) == -1)
			return (-1);
	}
	else
	{
		if (left_parent_execution(lists->pipes, lists->pipe_fd) == -1)
		{
			free_pipes(lists->pipes);
			if (lists->pipe_fd[0] > 0)
			{
				close(lists->pipe_fd[0]);
				close(lists->pipe_fd[1]);
			}
			return (-1);
		}
	}
	return (0);
}

int exec_pipe_right_execution(t_tree **ast, t_lists *lists, pid_t right_pid, pid_t left_pid)
{
	int exit_code;

	exit_code = 0;
	if (right_pid < 0)
	{
		// error
	}
	if (right_pid == 0)
	{
		if (right_child_execution(ast, lists, lists->pipe_fd, lists->pipes) == -1)
			return (-1);
	}
	else
	{
		close(lists->pipe_fd[0]);
		free_pipes(lists->pipes);
		exit_code = wait_children(right_pid, left_pid);
		return (exit_code);
	}
	return (0);
}

int malloc_error_close_free_pipes(int pipefd[2], t_pipe **pipes)
{
	if (close(pipefd[0]) == -1 || close(pipefd[1]) == -1)
	{
		free_pipes(pipes);
		return (-1);
	}
	free_pipes(pipes);
	return (-1);	
}

int exec_pipe_right_pipe_execution(t_tree **ast, t_lists *lists, pid_t left_pid)
{
	int exit_code;

	exit_code = exec_pipe(&((*ast)->right), lists);
	exit_code = wait_children(left_pid, left_pid);
	return (exit_code);
}

// DEFINE UN MAX PIPEFD ?
int	exec_pipe(t_tree **ast, t_lists *lists)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int exit_code;
	struct sigaction sa_ignore, sa_orig;

	setup_pipe_signals(&sa_ignore, &sa_orig);
	add_pipe(lists->pipe_fd, lists->pipes);
	left_pid = fork();
	if (exec_pipe_left_execution(ast, lists, left_pid) == -1)
		return (malloc_error_close_free_pipes(lists->pipe_fd, lists->pipes));
	if ((*ast)->right && (*ast)->right->token->token == PIPE)
		return (exec_pipe_right_pipe_execution(ast, lists, left_pid));
	else
	{
		right_pid = fork();
		exit_code = exec_pipe_right_execution(ast, lists, right_pid, left_pid);
		if (exit_code == -1)
			return (malloc_error_close_free_pipes(lists->pipe_fd, lists->pipes));
		sigaction(SIGINT, &sa_orig, NULL);
		update_last_arg_var(lists->env, (*ast)->token->content);
		return (exit_code);
	}
	return (1);
}

int exec_cmd_execve_child(t_tree **ast, t_lists *lists)
{
	char **strings_env;

	setup_child_signals();
	close_origin_fds(lists->origin_fds);
	strings_env = lst_to_array(lists->env);
	execve((*ast)->token->content[0], (*ast)->token->content,
		strings_env);
	perror("execve");
	free_array(strings_env);
	free_lists(lists);
	exit(-1);
}

int exec_cmd_execve(t_tree **ast, t_lists *lists)
{
	pid_t	pid;
	int exit_code;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (exec_cmd_execve_child(ast, lists) == -1)
			return (-1);
	}
	else
	{
		//doublon pour les signaux ?
		set_signals(1);
		exit_code = wait_children(pid, pid);
		update_last_arg_var(lists->env, (*ast)->token->content);
		setup_parent_signals();
		rl_on_new_line();
		return (exit_code);
	}
	return (1); //impossible d'arriver ici
}

int	exec_cmd(t_tree **ast, t_lists *lists)
{
	int		exit_code;
	char	*s;
	char	*tmp;

	if ((*ast)->token->token == BUILT_IN
		|| !ft_strncmp((*ast)->token->content[0], "source", 7))
	{
		exit_code = builtins((*ast)->token->content, lists);
		return (exit_code);
	}
	if (is_a_directory((*ast)->token->content[0]))
	{
		s = ft_strjoin("minishell: ", (*ast)->token->content[0]);
		tmp = s;
		s = ft_strjoin(s, ": Is a directory\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
		return (126);
	}
	if ((*ast)->token->token == CMD)
		return (exec_cmd_execve(ast, lists));
	return (1); // on ne devrait pas arriver ici
}

int	redirect_stdio(t_tree **ast, t_lists *lists)
{
	t_tree	*left;
	t_tree	*right;
	int		exit_code;
	char *file;

	// doute pour ce if, on devrait le faire au parsing ?
	if (!(*ast)->token->content[1])
		return (print_error_file_opening("", "syntax error\n", 2));
	left = (*ast)->left;
	right = (*ast)->right;
	if ((*ast)->token->token == HD)
		file = (*ast)->token->content[2];
	else
		file = (*ast)->token->content[1];
	exit_code = file_check(file, (*ast)->token->token, (*ast)->token->error);
	if (exit_code != 0)
		return (exit_code); // il faut return ici ?
	exit_code = open_dup2_close(file, (*ast)->token->token);
	if (exit_code == -1) // ERRNO
		return (-1);
	if (left && (left->token->token == DIREC && left->token->error == 127))
		exit_code = error_cmd(left->token->content[0], 127);
	if (right && (right->token->token == DIREC && right->token->error == 127))
		exit_code = error_cmd(right->token->content[0], 127);
	if (left && (left->token->token == R_IN || left->token->token == APPEND
			|| left->token->token == TRUNC || left->token->token == HD))
		exit_code = redirect_stdio(&left, lists);
	if (exit_code == 0 && right && (right->token->token == R_IN
			|| right->token->token == APPEND || right->token->token == TRUNC
			|| right->token->token == HD))
		exit_code = redirect_stdio(&right, lists);
	if (exit_code == 0 && left && (left->token->token == CMD
			|| left->token->token == BUILT_IN))
		exit_code = exec_cmd(&left, lists);
	if (exit_code == 0 && right && (right->token->token == CMD
			|| right->token->token == BUILT_IN))
		exit_code = exec_cmd(&right, lists);
	if ((*ast)->token->token == HD)
		unlink(file);
	return (exit_code);
}

int exec_boolop(t_tree **ast, t_lists *lists)
{
	int exit_code;

	exit_code = 0;
	if ((*ast)->left)
	{
		exit_code = exec_ast(&((*ast)->left), lists);
		if ((exit_code == 0 && (*ast)->token->token == O_AND) || 
			(exit_code != 0 && (*ast)->token->token == O_OR))
			return (exec_ast(&((*ast)->right), lists));
		else
			return (exit_code);
	}
	return (1); //cas ou left existe pas dans un node AND
}

//ici j'ai peut etre casse les signaux !!
int exec_parenthesis(t_tree **ast, t_lists *lists)
{
	pid_t pid;
	int exit_code;
	char **strings_env;
	t_tree *sub_ast;

	struct sigaction sa_ignore, sa_orig;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	exit_code = 0;
	pid = fork();
	if (!pid)
	{
		// error de fork
	}
	if (pid == 0)
	{
		close_origin_fds(lists->origin_fds);
		setup_child_signals();
		strings_env = lst_to_array(lists->env);
		sub_ast = parse((*ast)->token->group->content[0], strings_env, *lists->env, lists);
		free_array(strings_env);
		exit_code = exec_ast(&sub_ast, lists);
		free_tree(&sub_ast);
		free_lists(lists);
		exit (exit_code);
	}
	else
	{
		lists->exit_code = wait_children(pid, pid);
		sigaction(SIGINT, &sa_orig, NULL);
		/* printf("exit_code = %d\n", exit_code); */
		return (lists->exit_code);
	}
	return (1); // cas impossible ?
}

int exec_error_cases(t_tree **ast)
{
	if ((*ast)->token->error == 127 || (*ast)->token->error == 126
		|| (*ast)->token->error == 21)
		return (error_cmd((*ast)->token->content[0], (*ast)->token->error));
	if ((*ast)->token->error == 5)
	{
		ft_putstr_fd((*ast)->token->content[0], 2);
		return (5);
	}
	//c'est un syntax error ?
	return (-1); //quelle code d'erreur si on est entre dans aucun case ?
}

int exec_group_cmd(t_tree **ast, t_lists *lists)
{
	if ((*ast)->token->token == R_IN || (*ast)->token->token == APPEND
		|| (*ast)->token->token == TRUNC || (*ast)->token->token == HD)
		return (redirect_stdio(ast, lists));
	if ((*ast)->token->token == PIPE)
		return (exec_pipe(ast, lists));
	if ((*ast)->token->error != 126 && ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD
		|| !ft_strncmp((*ast)->token->content[0], "source", 7))) // exclure les codes d'erreurs
	{
		lists->exit_code = exec_cmd(ast, lists);
		return (lists->exit_code);
	}
	return (-1); // impossible d'arriver ici : quelle code d'erreur ?
}

int exec_group_boolop(t_tree **ast, t_lists *lists)
{
	char **strings_env;
	t_tree *sub_ast;

	sub_ast = NULL;
	strings_env = lst_to_array(lists->env);
	sub_ast = parse((*ast)->token->content[0], strings_env, *lists->env, lists);
	free_array(strings_env);
	lists->exit_code = exec_ast(&sub_ast, lists);
	free_tree(&sub_ast);
	return (lists->exit_code);
}

//retester les parentheses a la main :
//output
//error
//exit
//fds opens 
int	exec_ast(t_tree **ast, t_lists *lists)
{
	int			exit_code;
	t_alias		*alias;
	t_shell_fct	*shell_fct;

	exit_code = 0;
	if (!*ast)
		return (127); // on devrait peut etre reagir dans le main pour ca
	//signaux a virer ?
	struct sigaction sa_ignore, sa_orig;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_orig);
	//signaux a virer ?
	if ((*ast)->token->token == O_AND || (*ast)->token->token == O_OR)
		return (exec_boolop(ast, lists));
	if ((*ast)->token->token == GROUP_PARENTHESIS)
		return (exec_parenthesis(ast, lists));
	if ((*ast)->token->token == GROUP_BOOLOP)
		return (exec_group_boolop(ast, lists));	
	//l'error code 2 correspond a quoi deja ?
	if ((*ast)->token->error == 2)
	{
		ft_putendl_fd((*ast)->token->content[0], 2);
		return ((*ast)->token->error);
	}
	if (((*ast)->token->token == R_IN || (*ast)->token->token == APPEND || (*ast)->token->token == TRUNC) || 
		((*ast)->token->token == PIPE) || ((*ast)->token->token == HD) ||
		((*ast)->token->error != 126 && ((*ast)->token->token == BUILT_IN || (*ast)->token->token == CMD)))
		return (exec_group_cmd(ast, lists));
	//
	// un token Alias
	alias = is_a_known_alias((*ast)->token->content[0], lists->aliases);
	if ((*ast)->token->error == 127 && alias)
		return (exec_alias(ast, lists, alias));
	// un token shell_func
	shell_fct = is_a_known_shell_fct((*ast)->token->content[0],
			lists->shell_fcts);
	if (shell_fct)
	if ((*ast)->token->error == 127 && shell_fct)
		return (exec_shell_fct(ast, lists, shell_fct));
	return (exec_error_cases(ast));
}

// modifs pour le ctrl dans un pipe :
//
// il faut avoir un check is_in_pipe dans cmd : pour gerer un cat tout seul
// il faut deplacer les ajouts de set sig etc faites dans cmd dans exec_pipe
// il faut modifier la fonction wait_children pour qu'il recupere correctement le signal
// https://chat.deepseek.com/a/chat/s/a6fb8416-77db-418a-9c33-91607fa40c13

// Gros debuggage
//
// implementer le nouveau free de l'arbre
// corriger si on fait un ctrl D dans le prompt : tout free
// idem pour exit : tout free
//
// Parsing ?
// whoami | cat | > file1 | uname
// doit afficher Linux et pas cmd not found !
//
// TESTS
//
// PIPES
//
// whoami OK
// whoami | cat OK
// whoami | cat | cat OK
// whoami | cat | cat | cat OK
//
// REDIRS OUT
// whoami > file1 OK
// not_existing_cmd > file1 OK
// whoami > file1 > file2 OK
// whoami > file1 > file2 > file3 > file4 OK
// whoami >> file1 OK
// whoami > file1 >> file2 OK
// whoami >> file1 > file2 > file3
// whoami > file1 > file2 >> file3 KO
//
// REIDRS IN
//< file1 whoami KO
//< file1 cat : boucle infinie KO
//
// PIPE + redirs out
// whoami | cat | cat > file1 | cat > file1 > file2 | cat KO
//
// PIPE + redirs in
// TODO

// Mettre 1024 en limite de pipes !!
//
// gros BUG !
/* [Minishell]$ whoami | file1 */
/* mimishell: file1: Permission denied */
/* [Minishell]$ oelleaum */
/* mimishell: oelleaum: command not found */
/* [Minishell]$ 2004h[Minishell]$ */

// .c
/* int (t_tree **ast, t_var **env, t_pipe **pipes); */
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
