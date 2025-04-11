/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:35:23 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	*prompt;
	char	**arg;
	int		error_code;
	t_var	*new_env;

	/* t_ast *ast; */
	/* if (isatty(1)) */
	/* { */
	/* 	ft_putstr_fd("Minishell does not support non-interactive mode\n"); */
	/* 	exit(0); */
	/* } */
	(void)ac;
	(void)av;
	// pour ac et av : est-ce qu'on veut accepter des demarrages custom ?
	new_env = NULL;
	prompt = "[Minishell]$ ";
	/* utiliser getenv ?
		* Si on n'a pas d'env uniquement ?*/
	init_env(&new_env, env, av);
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{
			free(arg);
			free_list(&new_env);
			// a tester !
			exit(error_code);
		}
		arg = ft_split(line, ' ');
		/* ast = parser(line); */
		error_code = exec(arg, &new_env /* , ast */);
		free(line);
		line = NULL;
		ft_free(arg);
		arg = NULL;
	}
	free_list(&new_env);
	exit(error_code);
}
