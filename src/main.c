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

#include "libft.h"
#include <minishell.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdio.h>
#include <stdlib.h>


void	free_list(t_env **l)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = *l;
	if (!*l)
		return ;
	(*l)->prev->next = NULL;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp);
		free(tmp->line);
		tmp = next_node;
	}
	*l = NULL;
}

void	add_first_node(t_env **lst, t_env *new, char **env)
{
	*lst = new;
	new->prev = NULL;
	new->next = NULL;
	new->line = ft_strdup(env[0]);
}

void	add_back(t_env **lst, int n, char **env)
{
	t_env	*ptr;
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
	{
		free_list(lst);
		exit(1);
	}
	if (*lst == NULL)
		add_first_node(lst, new, env);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
		new->next = NULL;
		new->line = ft_strdup(env[n]);
	}
}

int	init_env(t_env **new_env, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		add_back(new_env, i, env);
		i++;
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	*prompt;
	char	**arg;
	int		error_code;
	t_env	*new_env;
	/* t_env	*tmp; */

	/* int i; */
	/* i = 0; */
	// isatty : pour
	(void)ac;
	(void)av;
	new_env = NULL;
	prompt = "[Minishell]$ ";
	if (*env)
		init_env(&new_env, env);
	else 
		new_env = NULL;
	/* else  */
	// init un env vide
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
			exit(error_code);
		/* printf("%s\n", line); */
		/* parser(line); */
		/* exec(line); */
		arg = ft_split(line, ' ');
		/* while(arg[i]) */
		/* { */
		/* 	printf("%s\n", arg[i]); */
		/* 	i++; */
		/* } */
		error_code = exec(arg, &new_env);
		free(line);
		ft_free(arg);
	}
	exit(error_code);
}
