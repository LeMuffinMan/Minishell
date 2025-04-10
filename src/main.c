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
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
	*l = NULL;
}

void set_node(t_env *node, int mode)
{
	if (mode == 1)
	{
		node->env = 1;
		node->exported = 0;
	}
	if (mode == 2)
	{
		node->env = 0;
		node->exported = 1;
	}
	if (mode == 3)
	{
		node->env = 1;
		node->exported = 1;
	}
	else
	{
		node->env = 0;
		node->exported = 0;
	}
}

void	add_first_node(t_env **lst, t_env *new, char *s, int mode)
{
	char **key_value;

	key_value = ft_split(s, '=');
	new->key = ft_strdup(key_value[0]);
	if (key_value[1])
		new->value = ft_strdup(key_value[1]);
	ft_free(key_value);
	*lst = new;
	new->prev = NULL;
	new->next = NULL;
	set_node(new, mode);
}

void	add_back(t_env **lst, char *s, int mode) // 0 = aucun des deux / 1 = env / 2 = export / 3 = env + export 
{
	t_env	*ptr;
	t_env	*new;
	char **key_value;

	new = malloc(sizeof(t_env));
	if (new == NULL)
	{
		/* free_list(lst); */
		exit(139);
	}
	if (*lst == NULL)
		add_first_node(lst, new, s, mode);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
		new->next = NULL;
		key_value = ft_split(s, '=');
		new->key = ft_strdup(key_value[0]);
		if (key_value[1])
			new->value = ft_strdup(key_value[1]);
		ft_free(key_value);
		set_node(new, mode);
	}
}
//lst_utils





int init_last_cmd_var(char *name, t_env **env)
{
	char *s;

	s = NULL;
	s = ft_strjoin("_=/usr/bin/./", name, s);
	add_back(env, s, 1);
	free(s);
	return (0);
}

int build_minimal_env(t_env **env, char **arg)
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
	add_back(env, s, 3);
 	free(s);
  s = ft_strdup("SHLVL=1");
	add_back(env, s, 3);
	free(s);
	s = ft_strdup("OLDPWD"); //updated in CD seule;ent ?
	add_back(env, s, 2);
	free(s);
	init_last_cmd_var(arg[0], env);
	return (0);
}

//a tester 
int init_and_incremente_shlvl(char *s, t_env **env)
{
	int n;
	int i;
	char *line;
	char *shlvl_n;

	i = 7;
	line = NULL;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			break;
		i++;
	}
	if ((size_t)i == ft_strlen(s))
	{
		n = ft_atoi(s + 6);
		if (n < 0)
			line = ft_strdup("SHLVL=0");
		else 
		{
			shlvl_n = ft_itoa(n + 1);
			line = ft_strjoin("SHLVL=", shlvl_n, shlvl_n); 
		}
	}
	if (!line)
		line = ft_strdup("SHLVL=1");
	add_back(env, line, 3);
	free(line);
	return (0);
}

//les add back marchent pas du tout !
int	init_env(t_env **new_env, char **env, char **arg)
{
	int	i;

	//pour simuler un environnement absent et tester les leaks
	/* *env = NULL; */
	if (!*env)
		return(build_minimal_env(new_env, arg));	
	i = 0;
	while (env[i])
	{
		//des choses a revoir sur le shell level !
		//on doit gerer SHLVL+=1 ?
		if (!ft_strncmp("SHLVL=", env[i], 6))
			init_and_incremente_shlvl(env[i], new_env);
		else
			add_back(new_env, env[i], 3); //pas sur
		/* t_env *tmp; */
		/**/
		/* tmp = *new_env; */
		/* while(tmp) */
		/* 	tmp = tmp->next; */
		/* printf("%s=%s\n", tmp->key, tmp->value); */
		i++;
	}
	init_last_cmd_var(arg[0], new_env);
	return (0);
}

//init

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
	init_env(&new_env, env, av);
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
