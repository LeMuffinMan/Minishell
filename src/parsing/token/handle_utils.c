/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:54:05 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/24 01:18:45 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

bool	is_valid_argcmd(t_token *node)
{
	struct stat	status;

	if (node->content[0][0] == '\0')
		return (false);
	else if (stat(node->content[0], &status) == -1)
		return (false);
	else if (S_ISREG(status.st_mode)
		|| S_ISDIR(status.st_mode))
		return (false);
	return (true);
}

static bool	verif_pwd(char **path_splited, char *name_dir)
{
	int		i;
	bool	flag;

	i = 0;
	flag = true;
	while (path_splited[i])
	{
		if (ft_strcmp(path_splited[i], name_dir) == 0)
		{
			flag = false;
			break ;
		}
		i++;
	}
	free_tab(path_splited);
	free(name_dir);
	return (flag);
}

bool	is_pwd_valid(t_var *list_env)
{
	char	*name_dir;
	char	**path_splited;
	bool	flag;

	flag = true;
	name_dir = getcwd(NULL, 0);
	if (!name_dir)
		return (false);
	while (list_env && ft_strcmp(list_env->key, "PATH") != 0)
		list_env = list_env->next;
	if (!list_env || !list_env->value)
	{
		free(name_dir);
		return (true);
	}
	path_splited = ft_split(list_env->value, ':');
	if (!path_splited)
	{
		free(name_dir);
		return (true);
	}
	flag = verif_pwd(path_splited, name_dir);
	return (flag);
}
