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

#include <readline/readline.h> // compiler avec -l readline
#include <stdio.h>
#include <stdlib.h>
#include <minishell.h>

int	main(void)
{
	char	*line;
	char	*prompt;

  //isatty : pour 
	prompt = "[Minishell]$ ";
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
      exit (1);
		/* printf("%s\n", line); */
		/* parser(line); */
		/* exec(line); */
		exec();
		free(line);
	}
	return (0);
}
