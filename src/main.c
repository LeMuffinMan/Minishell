/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/21 13:35:04 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "display.h"
#include "env_utils.h"
#include "exec.h"
#include "get_prompt.h"
	// A VIRER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "history.h"
#include "init.h"
#include "libft.h"
#include "minishellrc.h"
#include "misc.h"
#include "prints.h"
#include "signals.h"
#include "tree.h"
#include "utils.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdlib.h>
#include <unistd.h>

// Exec todo
// Bultins refacto
// unset SHLVL USER HOME PATH export | grep SHLVL export | grep USER export | grep HOME export | grep PATH : STDERR
	/* + EXIT fails */

// pour le merge de mardi 13 mai
// exec clean
// builtins refacto
//()
// here_docs
// bug fixs
// wildcards
// A tester
// la nouvelle exec avec ()
// builtins
// here_docs (signaux !!)
// wildcards
// signaux

// Merge d'apres (20 mai)
// cd + pwd builtin error to fix
// debuggage nouvelle exec + ()
// debuggage Here_docs + quotes + expands
// debuggage wildcard
// exec malloc protection
// normed

// Push autour du 26 mai ?

// ultrabonus :
// tester load minishellrc avec le parsing fini
// proteger get_prompt
// alias :
// token d'alias au parsing ?
// shell_fct :
// token shlfct au parsing ?
// history
// tester builtin
// tester ! cmds

// Tests de cons :
// CTRL V + Tab : fait un tab dans le minishell a gerer !

// TODO pour merge 20 mai
// exec :
// DONE integrer nouvelle exec && || ()
// DONE fixer le builtin exit
// DONE fixer cd et proteger cd - et getcwd a l'init
// malloc protection de tout exec.c
// normage exec.c
//
// DONE virer autocompletion
// parsing :
// minor fix () && ||
// bug fixes
// ajouter les fixs en reserve ( $HONE $USER ) + (unset )
// update $? avec l'exit_code dans lists
// here_doc (expand dans here_doc ?)
// wildcards

// A TESTER AU MERGE
// la nouvelle exec
//    Avec parentheses
//    avec sous parentheses
//    avec tous dans la meme ligne
//    Leaks
//
// here_docs
//		unlink
//		signaux
//
// wildscards
//	4 patterns
//
// BUILTINS
// SIGNAUX
//
// TODO POUR LE 26 mai
// BUG FIXES de ce qui a merde au merge, et des cas de cons
// gitaction pour proteger le push sur master
// norm & malloc protection check
//
// CTRL + V et TAB
// ou juste TAB
//
// push 26 mai

//Vendedi 23 merge vant 16h :
//parsing :
 	//done fix sudo -A et sudo -A apt update
	//done echo $HOME $USER
 	//fixer export : il faut toujours maitnenir dnas un seul char * chaque couple key=value
//
 	//done HERE_DOC 
 	//wildcards 
//Exec :
 	//Refacto de tout exec.c 
//
	//fix exec here_doc + signaux
  //reparer et tester les builtins





int	malloc_error_close_free_exit(t_lists *lists)
{
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(errno);
}

//ULTRABONUS
int free_prompt_string(char *prompt)
{
	if (prompt && ft_strncmp(prompt, "[Minishell]$ ", 13))
	{
		free(prompt);
		prompt = NULL;
	}
	return (0);
}

//ULTRABONUS
int	init_prompt(char **strenv, char **prompt, t_var **env)
{
	if (isatty(0) && *strenv)
	{
		free_prompt_string(*prompt);
		*prompt = get_prompt(env);
		if (!*prompt)
			return (-1);
	}
	else if (!*prompt)
	{
		*prompt = "[Minishell]$ ";
	}
	return (0);
}

int	end_of_file_exit(char **prompt, t_lists *lists, int exit_code)
{
	//Ultrabonus
	if (save_history(lists->env, lists->history) == -1)
		malloc_error_close_free_exit(lists);
	free_prompt_string(*prompt);
	if (lists->origin_fds[0] != -1 && close(lists->origin_fds[0]) == -1)
		malloc_error_close_free_exit(lists);
	if (lists->origin_fds[1] != -1 && close(lists->origin_fds[1]) == -1)
		malloc_error_close_free_exit(lists);
	free_lists(lists);
	exit(exit_code); // Normal exit when readline returns NULL (Ctrl+D)
}

int update_history(char *line, t_lists *lists, char **env)
{
	if (ft_strlen(line) > 0 && !is_duplicated_hist_entry(lists->history,
				line))
	{
		if (isatty(0) && *env)
		{
			if (ft_add_history(lists->env, lists->history, line) == -1)
				return (-1);
		}
		else
			add_history(line);
	}
	return (0);
}

char	*readline_loop(char **prompt, t_lists *lists, char **env, int exit_code)
{
	char	*line;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	while (ft_strlen(line) == 0 || !ft_strncmp(line, "\n", 2))
	{
		free(line);
		line = NULL;
		line = readline(*prompt);
		if (!line || !line)
			end_of_file_exit(prompt, lists, exit_code);
		if (update_history(line, lists, env) == -1)
		{
			free_prompt_string(*prompt);
			free(line);
			malloc_error_close_free_exit(lists);
		}
	}
	return (line);
}

int	dup_origins_fds(int origin_fds[2])
{
	origin_fds[0] = dup(STDIN_FILENO);
	if (origin_fds[0] == -1)
	{
		return (-1);
	}
	origin_fds[1] = dup(STDOUT_FILENO);
	if (origin_fds[1] == -1)
	{
		return (-1);
	}
	return (0);
}

int	clean_post_execution(int origin_fds[2], t_lists *lists, char **line)
{
	t_tree	*tree_to_free;

	if (dup2(origin_fds[0], STDIN_FILENO) == -1)
		return (-1);
	if (dup2(origin_fds[1], STDOUT_FILENO) == -1)
		return (-1);
	if (close_origin_fds(origin_fds) == -1)
		return (-1);
	origin_fds[0] = -1;
	origin_fds[1] = -1;
	if (lists->ast && *lists->ast)
	{
		tree_to_free = *lists->ast;
		free_tree(&tree_to_free);
		*lists->ast = NULL;
	}
	free(*line);
	*line = NULL;
	return (0);
}

int	parse_and_execution_loop(char **env, char **prompt, t_lists *lists,
		int exit_code)
{
	char	**strings_env;
	char	*line;

	setup_parent_signals();
	if (init_prompt(env, prompt, lists->env) == -1)
		malloc_error_close_free_exit(lists);
	line = readline_loop(prompt, lists, env, exit_code);
	if (!line)
		malloc_error_close_free_exit(lists);
	if (dup_origins_fds(lists->origin_fds) == -1)
		malloc_error_close_free_exit(lists);
	//est-ce qu'on a besoin du strings env ou on peut passer direct les lists ou la liste env ?
	strings_env = lst_to_array(lists->env);
	(*lists->ast) = parse(line, strings_env, *lists->env, lists);
	/* if (errno = ENOMEM) // tant qu'on detecte le malloc qui foire avec un if (str == NULL), on peut faire remonter l'erreur avec un if (errno == ENOMEM)  */
	/* 	malloc_error_close_free_exi et(lists); */
	free_array(strings_env);
	strings_env = NULL;
	exit_code = exec_ast(lists->ast, lists);
	/* if (errno = ENOMEM) */
	/* 	malloc_error_close_free_exit(lists); */
	// a virer : exit code dans la structure lists ?
	update_exit_code_var(lists->env, exit_code);
	// a virer
	if (clean_post_execution(lists->origin_fds, lists, &line) == -1)
		malloc_error_close_free_exit(lists);
	return (exit_code);
}

int	main(int ac, char **av, char **env)
{
	char	*prompt;
	int		exit_code;
	t_lists	lists;

	if (ac != 1)
	{
		ft_putstr_fd("Minishell does not accept arguments\n", 2);
		exit(1);
	}
	prompt = NULL;
	// decommenter a la toute fin !!!
	/* is_interactive_mode(); */
	exit_code = 0;            
		// on ajoute l'exit code a la megastruct ou on la laisse dans env ?
	rl_inhibit_completion = 1; // desactive l'autocompletion
	if (init(&lists, av, env) == -1)
	{
		free_lists(&lists);
		return (errno);
	}
	while (1)
		exit_code = parse_and_execution_loop(env, &prompt, &lists, exit_code);
	//superful ?
	free_lists(&lists);
	return (errno);
}
