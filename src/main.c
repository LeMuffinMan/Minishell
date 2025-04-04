/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/01 17:28:36 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdlib.h>
#include <minishell.h>
#include "libft.h"

int	main(void)
{
	char	*line;
	char	*prompt;
	char 	**arg;
	/* int i; */

	/* i = 0; */
  //isatty : pour 
	prompt = "[Minishell]$ ";
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
      exit (0);
		/* printf("%s\n", line); */
		/* parser(line); */
		/* exec(line); */

		arg = ft_split(line, ' ');
		/* while(arg[i]) */
		/* { */
		/* 	printf("%s\n", arg[i]); */
		/* 	i++; */
		/* } */
		exec(arg);
		free(line);
		ft_free(arg);
	}
	return (0);
}
