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

#include "libft.h"
#include "structs.h"
#include "utils.h"
#include "tree.h"
#include <unistd.h>

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

int exit_no_arg(t_var **env, t_tree **ast, int origin_fds[2])
{
	free_list(env);
	free_tree(&(*ast)->head);
	//free le prompt ici ?
  /* free_history(history); */
	ft_putstr_fd("exit\n", 1);
	close_origin_fds(origin_fds);
	free_lists(lists);
	exit(0);
}

int exit_overflow_error(char **arg)
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: exit: ", arg[1]);
	tmp = s;
	s = ft_strjoin(s, ": numeric argument required\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (2);
}

int exit_numeric_argument_required_error(char **arg, t_var **env, t_tree **ast, int origin_fds[2])
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: exit: ", arg[1]);
	tmp = s;
	s = ft_strjoin(s, "numeric argument required\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	free_list(env);
	free_tree(&(*ast)->head);
	free_lists(lists);
	//free le prompt ici ?
  /* free_history(history); */
	close_origin_fds(origin_fds);
	exit(2);
}

int exit_with_valid_arg(char **arg, t_var **env, t_tree **ast, int origin_fds[2])
{
	int n;

	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	free_list(env);
	free_tree(&(*ast)->head);
	//free le prompt ici ?
  /* free_history(history); */
	close_origin_fds(origin_fds);
	free_lists(lists);
	exit(n);
}

// a voir
// des fuites et des exits a des moments ou ils doivent pas
// a tester : is only numeric arg
int	builtin_exit(char **arg, t_var **env, t_tree **ast, int origin_fds[2])
{
	if (!arg[1])
		exit_no_arg(env, ast, origin_fds);
	if (ft_strlen(arg[1]) > 18)
		return (exit_overflow_error(arg));
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!is_only_numeric_argument(arg[1]))
		return(exit_numeric_argument_required_error(arg, env, ast, origin_fds));
	exit_with_valid_arg(arg, env, ast, origin_fds);
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
