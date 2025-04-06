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
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>


void	free_list(t_env **l)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = *l;
	if (!*l)
		return ;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->line);
		free(tmp);
		tmp = next_node;
	}
	*l = NULL;
}

void	add_first_node(t_env **lst, t_env *new, char *s)
{
	*lst = new;
	new->prev = NULL;
	new->next = NULL;
	new->line = ft_strdup(s);
}

void	add_back(t_env **lst, char *s)
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
		add_first_node(lst, new, s);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
		new->next = NULL;
		new->line = ft_strdup(s);
	}
}

int build_minimal_env(t_env **env)
{
	char *s;
	char	buf[PATH_MAX];

	s = NULL;
	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strdup(buf);
	else
	{
		perror("getcwd");
		/* (void)s = NULL; */
		//  1 ou errno ?
	//  free avant !
		return (1);
	}
	s = ft_strjoin("PWD=", s, s);
	add_back(env, s);
 	free(s);
  s = ft_strdup("SHLVL=1");
	add_back(env, s);
	free(s);
	s = ft_strdup("_=/usr/bin/env"); //A CHANGER !
	add_back(env, s);
	free(s);
	return (0);
}

int	init_env(t_env **new_env, char **env)
{
	int	i;

	if (!*env)
	{
		build_minimal_env(new_env);	
	/* PWD=/home/muffin */
	/* SHLVL=1 */
	/* _=/usr/bin/env */
	}
	i = 0;
	while (env[i])
	{
		add_back(new_env, env[i]);
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
/* utiliser getenv ?*/
	/* int i; */
	/* i = 0; */
	// isatty : pour
	(void)ac;
	(void)av;
	new_env = NULL;
	prompt = "[Minishell]$ ";
	init_env(&new_env, env);
	/* else  */
	// init un env vide
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{
			free(arg);
			free_list(&new_env);
			exit(error_code);
		}
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
		line = NULL;
		ft_free(arg);
		arg = NULL;
	}
	free_list(&new_env);
	exit(error_code);
}
