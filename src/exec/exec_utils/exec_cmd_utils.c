/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:34:42 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:34:46 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tree.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int	directory_error(char **s, char *tmp)
{
	int	exit_code;

	*s = ft_strjoin(*s, ": Is a directory\n");
	exit_code = 126;
	if (errno == ENOMEM)
	{
		free(tmp);
		return (errno);
	}
	ft_putstr_fd(*s, 2);
	free(tmp);
	free(*s);
	return (exit_code);
}

int	missing_file_error(char **s, char *tmp)
{
	int	exit_code;

	*s = ft_strjoin(*s, ": No such file or directory\n");
	exit_code = 127;
	if (errno == ENOMEM)
	{
		free(tmp);
		return (errno);
	}
	ft_putstr_fd(*s, 2);
	free(tmp);
	free(*s);
	return (exit_code);
}

int	exec_cmd_print_error(t_tree **ast)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: ", (*ast)->token->content[0]);
	if (errno == ENOMEM)
		return (errno);
	tmp = s;
	if (access((*ast)->token->content[0], F_OK) == 0)
		return (directory_error(&s, tmp));
	else
		return (missing_file_error(&s, tmp));
}
