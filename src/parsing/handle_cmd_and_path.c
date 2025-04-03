/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 23:46:28 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 02:04:41 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "libft.h"
#include <unistd.h>

bool	parse_path_without_env(char *str)
{
	char	**tmp_cmd;

	tmp_cmd = ft_split(str, ' ');
	if (!tmp_cmd || !tmp_cmd[0])
	{
		free_tab(tmp_cmd);
		free(str);
		str = NULL;
		return (false);
	}
	if (!verif_access_exec(tmp_cmd[0]))
	{
		free_tab(tmp_cmd);
		return (false);
	}
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

bool	verif_access_exec(char *tmp_cmd)
{
	int	i;

	i = access(tmp_cmd, F_OK);
	if (i == 0)
	{
		i = access(tmp_cmd, X_OK);
		if (i != 0)
			return (false);
	}
	else
		return (false);
	return (true);
}

char	*verif_path(t_pipex *pipex, char **path, char *cmd)
{
	int		i;
	char	*tmp_cmd;
	char	*tmp_path;

	i = 0;
	tmp_cmd = NULL;
	tmp_path = NULL;
	while (path[++i] && pipex->error != PERMISSION_DENIED)
	{
		tmp_path = ft_strjoin(path[i], "/");
		if (!tmp_path)
			return (NULL);
		tmp_cmd = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (!tmp_cmd)
			return (NULL);
		if (verif_access_exec(pipex, tmp_cmd))
			break ;
		else
		{
			free(tmp_cmd);
			tmp_cmd = NULL;
		}
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
	if (!new_path || !new_path[0])
	{
		free_tab(new_path);
		return (NULL);
	}
	return (new_path);
}

char	*extract_cmd(char *arg, char **path)
{
	char	*cmd;
	char	**split_cmd;

	cmd = NULL;
	split_cmd = ft_split(arg, ' ');
	if (!split_cmd || !split_cmd[0])
	{
		free_tab(path);
		if (split_cmd && !split_cmd[0])
		{
			free_tab(split_cmd);
			ft_error("Command not found", CMD_NOT_FOUND);
		}
		else
			ft_error("Problem with the split of 'extract_cmd'", MEM_ALLOC);
	}
	cmd = verif_path(pipex, path, split_cmd[0]);
	if (!cmd)
		problem_cmd(pipex, path, split_cmd);
	free_tab(path);
	free_tab(split_cmd);
	return (cmd);
}
