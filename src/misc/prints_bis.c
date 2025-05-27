/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:44:20 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:48:43 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include "malloc_error_handlers.h"

int	print_error_file_opening(char *file, char *error, int error_code)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: ", file);
	tmp = s;
	s = ft_strjoin(s, error);
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (error_code);
}

int	error_not_valid_identifier(char *s)
{
	char	*tmp;

	s = ft_strjoin("minishell: export: `", s);
	tmp = s;
	s = ft_strjoin(s, "\': not a valid identifier\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (1);
}

int	print_error_is_a_directory(char *file)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: ", file);
	tmp = s;
	s = ft_strjoin(s, ": Is a directory\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (1);
}

int	print_perm_error(char *file)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: ", file);
	tmp = s;
	s = ft_strjoin(s, ": Permission denied\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (1);
}

int	error_cmd_is_a_directory(char *cmd)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("minishell: ", cmd);
	if (errno == ENOMEM)
		return (errno);
	tmp = s;
	s = ft_strjoin(s, ": Is a directory\n");
	if (errno == ENOMEM)
		return (malloc_error_cmd(tmp));
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (126);
}
