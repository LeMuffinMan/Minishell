/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 23:46:28 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/02 23:36:49 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include <unistd.h>

bool	parse_path_without_env(t_token *node)
{
	char	**tmp_cmd;

	tmp_cmd = ft_split(node->content[0], ' ');
	if (!tmp_cmd)
	{
		free_tab(tmp_cmd);
		free_parse(node,
			"Malloc failed in function 'parse_path_whithout env'",
			MEM_ALLOC);
	}
	if (!tmp_cmd[0])
	{
		free_tab(tmp_cmd);
		return (false);
	}
	if (!verif_access_exec(tmp_cmd[0], &node->error))
	{
		free_tab(tmp_cmd);
		return (false);
	}
	free_tab(tmp_cmd);
	return (true);
}

char	*extract_path(char **envp)
{
	char	*res;
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	res = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			res = ft_strjoin(tmp, envp[i]);
			if (!res)
			{
				free(tmp);
				return (NULL);
			}
			free(tmp);
			break ;
		}
		i++;
	}
	return (res);
}

static char	*verif_path(char **path, char *cmd, int *error)
{
	int		i;
	char	*tmp_cmd;
	char	*tmp_path;

	i = 0;
	while (path[i] && *error != PERMISSION_DENIED)
	{
		tmp_path = ft_strjoin(path[i], "/");
		if (!tmp_path)
			return (NULL);
		tmp_cmd = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (!tmp_cmd)
			return (NULL);
		if (verif_access_exec(tmp_cmd, error))
			break ;
		free(tmp_cmd);
		tmp_cmd = NULL;
		i++;
	}
	return (tmp_cmd);
}

char	**split_the_path(char *path)
{
	char	**new_path;
	char	*tmp;
	int		i;

	i = 0;
	while (path[i] && path[i] != '=')
		i++;
	i += 1;
	tmp = ft_strdup(&path[i]);
	if (!tmp)
		return (NULL);
	new_path = ft_split(tmp, ':');
	free(tmp);
	if (!new_path)
	{
		free_tab(new_path);
		return (NULL);
	}
	return (new_path);
}

char	*parse_cmd(char *arg, char **path, int *error, bool flag)
{
	char	*cmd;
	char	**split_cmd;

	cmd = NULL;
	split_cmd = NULL;
	if (flag)
	{
		alloc_cmd_split(&split_cmd, path, arg, error);
		if (*error == CMD_NOT_FOUND)
			return (NULL);
	}
	if (flag)
		cmd = verif_path(path, split_cmd[0], error);
	else
		cmd = verif_path(path, arg, error);
	free_tab(path);
	free_tab(split_cmd);
	return (cmd);
}
