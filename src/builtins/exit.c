/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:36:33 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 18:20:04 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exit_utils.h"
#include "libft.h"
#include "malloc_error_handlers.h"
#include <errno.h>
#include <unistd.h>

int	exit_no_arg(t_lists *lists)
{
	ft_putstr_fd("exit\n", 1);
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	exit(0);
}

int	exit_overflow_error(t_lists *lists, char **arg)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: exit: ", arg[1]);
	if (!s)
		return (errno);
	tmp = s;
	s = ft_strjoin(s, ": numeric argument required\n");
	if (!s)
		builtin_exit_malloc_error(lists, tmp);
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	exit (2);
}

int	exit_numeric_argument_required_error(char **arg, t_tree **ast,
		t_lists *lists)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: exit: ", arg[1]);
	if (!s)
		builtin_exit_malloc_error(lists, NULL);
	tmp = s;
	s = ft_strjoin(s, "numeric argument required\n");
	if (!s)
		builtin_exit_malloc_error(lists, tmp);
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	free_lists(lists);
	close_origin_fds(lists->origin_fds);
	(void)ast;
	exit(2);
}

int	exit_with_valid_arg(char **arg, t_tree **ast, t_lists *lists)
{
	int	n;

	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	close_origin_fds(lists->origin_fds);
	free_lists(lists);
	(void)ast;
	exit(n);
}

int	builtin_exit(char **arg, t_tree **ast, t_lists *lists)
{
	if (!arg[1])
		exit_no_arg(lists);
	if (ft_strlen(arg[1]) > 18)
		return (exit_overflow_error(lists, arg));
	if (!is_only_numeric_argument(arg[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		free_lists(lists);
		close_origin_fds(lists->origin_fds);
		exit(2);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!is_only_numeric_argument(arg[1]))
		return (exit_numeric_argument_required_error(arg, ast, lists));
	exit_with_valid_arg(arg, ast, lists);
	return (0);
}
