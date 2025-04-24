/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:36:33 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:36:33 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "structs.h"
#include "tree.h"
#include <stdio.h>
/* #include <unistd.h> */
#include <stdlib.h>

// Traps ???

int	is_only_numeric_argument(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if ((s[i] > '9' || s[i] < '0') && (s[i] != '-' && s[i] != '+'))
			return (0);
		i++;
	}
	return (1);
}

// a voir
// des fuites et des exits a des moments ou ils doivent pas
// a tester : is only numeric arg
int	builtin_exit(char **arg, t_var **env, t_tree **ast)
{
	int		n;
	char	*s;
	char	*tmp;

	(void)env;
	s = NULL;
	if (!arg[1])
	{
		free_list(env);
		free_tree(ast);
		ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	if (ft_strlen(arg[1]) > 18)
	{
		s = ft_strjoin("minishell: exit: ", arg[1]);
		tmp = s;
		s = ft_strjoin(s, ": numeric argument required\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
		return (2);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!is_only_numeric_argument(arg[1]))
	{
		s = ft_strjoin("minishell: exit: ", arg[1]);
		tmp = s;
		s = ft_strjoin(s, "numeric argument required\n");
		free(tmp);
		ft_putstr_fd(s, 2);
		free(s);
		free_list(env);
		free_tree(ast);
		exit(2);
	}
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	free_list(env);
	free_tree(ast);
	exit(n);
	return (0);
}
/* Exit status: */
/*        125    if the env command itself fails */
/**/
/*        126    if COMMAND is found but cannot be invoked */
/**/
/*        127    if COMMAND cannot be found */
/**/
/*        -      the exit status of COMMAND otherwise */
