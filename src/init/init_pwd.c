/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:39:58 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:41:41 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include "libft.h"
#include "limits.h"
#include <unistd.h>

static int	handle_pwd_success(char *buf, t_var **env)
{
	char	*s;
	char	*tmp;

	s = ft_strjoin("PWD=", buf);
	if (!s)
		return (-1);
	tmp = s;
	s = ft_strjoin(s, "\n");
	if (!s)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	add_back_var(env, s, 3);
	return (0);
}

static void	print_pwd_error(void)
{
	ft_putstr_fd("shell-init: error retrieving current directory: getcwd: \
		cannot access parent directories: No such file or directory\n", 2);
}

int	init_pwd(t_var **env)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
		return (handle_pwd_success(buf, env));
	else
	{
		print_pwd_error();
		return (1);
	}
}
