/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/29 20:21:25 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parse_and_execution_loop.h"
#include "signals.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	end_of_file_exit(t_lists *lists, int exit_code)
{
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	write(1, "exit\n", 5);
	exit(exit_code);
}

char	*readline_loop(char **prompt, t_lists *lists, int exit_code)
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
			end_of_file_exit(lists, exit_code);
		if (ft_strlen(line) > 0)
			add_history(line);
	}
	return (line);
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

int	parse_and_execution_loop(char **prompt, t_lists *lists, int exit_code)
{
	char	*line;

	setup_parent_signals();
	line = readline_loop(prompt, lists, exit_code);
	if (!line)
		malloc_error_close_free_exit(lists);
	collect_signal_exit_code(lists);
	if (dup_origins_fds(lists->origin_fds) == -1)
		malloc_error_close_free_exit(lists);
	(*lists->ast) = parse(line, *lists->env, lists);
	if (errno == ENOMEM)
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
	t_lists	lists;

	// is_interactive_mode();
	if (ac != 1)
	{
		ft_putstr_fd("Minishell does not accept arguments\n", 2);
		exit(1);
	}
	prompt = "[Minishell]$ ";
	lists.exit_code = 0;
	g_signal = 0;
	if (init(&lists, av, env) == -1)
	{
		free_lists(&lists);
		return (errno);
	}
	while (1)
		lists.exit_code = parse_and_execution_loop(&prompt, &lists, lists.exit_code);
	free_lists(&lists);
	return (lists.exit_code);
}
