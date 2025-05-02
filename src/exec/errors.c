/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:27:24 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:29:16 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int error_cmd(char *cmd, int error_code)
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: ", cmd);	
	tmp = s;
	if (error_code == 126)
		s = ft_strjoin(s, ": Permission denied\n");
	if (error_code == 127)
		s = ft_strjoin(s, ": command not found\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	if (error_code == 126)
		return (126);
	return (127);
}

int print_error_file_opening(char *file, char *error)
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: ", file);
	tmp = s;
	s = ft_strjoin(s, error);
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (1);
}
