/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:44:32 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_prompt.h"
	// A VIRER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "history.h"    //
#include "libft.h"
#include "minishellrc.h" //
#include "parse_and_execution_loop.h"
#include "signals.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Mardi 14h merge
// Parsing
//- fix syntax error parenthesis
//- fix echo espaces en trop ou en moins
//- fix echo $
//- fix redir sur l'ast (les redirs a droite)
//- fix unset PATH qui casse la reco des builtins
//- fix ls considere comme file
// exec
//- fix minishell: ./usr/bin/sudo: Is a directory : si execve renvoie 2 (?) imprimer bash: ./usr/bin/sudo: No such file or directory

int	free_prompt_string(char *prompt)
{
	// ULTRABONUS
	if (prompt && ft_strncmp(prompt, "[Minishell]$ ", 13))
	{
		free(prompt);
		prompt = NULL;
	}
	return (0);
}

int	init_prompt(char **strenv, char **prompt, t_var **env)
{
	// ULTRABONUS
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
	// Ultrabonus
	if (save_history(lists->env, lists->history) == -1)
		malloc_error_close_free_exit(lists);
	free_prompt_string(*prompt);
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(exit_code); // Normal exit when readline returns NULL (Ctrl+D)
}

int	update_history(char *line, t_lists *lists, char **env)
{
	// ULTRABONUS
	if (ft_strlen(line) > 0 && !is_duplicated_hist_entry(lists->history, line))
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
		if (!line)
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
	char	*line;

	setup_parent_signals();
	if (init_prompt(env, prompt, lists->env) == -1)
		malloc_error_close_free_exit(lists);
	line = readline_loop(prompt, lists, env, exit_code);
	if (!line)
		malloc_error_close_free_exit(lists);
	collect_signal_exit_code(lists);
	if (dup_origins_fds(lists->origin_fds) == -1)
		malloc_error_close_free_exit(lists);
	if (*prompt && ft_strncmp(*prompt, "[Minishell]$ ", 13))
	{
		free(*prompt);
		*prompt = NULL;
		prompt = NULL;
	}
	(*lists->ast) = parse(line, *lists->env, lists);
	if (errno == ENOMEM)
		// tant qu'on detecte le malloc qui foire avec un if (str == NULL),
		/* on peut faire remonter l'erreur avec un if (errno == ENOMEM) */
		malloc_error_close_free_exit(lists);
	exit_code = exec_ast(lists->ast, lists);
	if (errno == ENOMEM)
		malloc_error_close_free_exit(lists);
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
	g_signal = 0;
	if (init(&lists, av, env) == -1)
	{
		free_lists(&lists);
		return (errno);
	}
	while (1)
		exit_code = parse_and_execution_loop(env, &prompt, &lists, exit_code);
	// superful ?
	free_lists(&lists);
	return (exit_code);
}
