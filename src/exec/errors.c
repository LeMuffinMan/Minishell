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

int error_cmd_not_found(char *cmd)
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: ", cmd);	
	tmp = s;
	s = ft_strjoin(s, ": command not found\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (127);
}

//a tester !!
int error_cmd_perm_denied(char *cmd)
{
	char *s;
	char *tmp;

	s = ft_strjoin("minishell: ", cmd);	
	tmp = s;
	s = ft_strjoin(s, ": Permission denied\n");
	free(tmp);
	ft_putstr_fd(s, 2);
	free(s);
	return (126);
}


