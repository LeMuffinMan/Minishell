/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_path_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:53:37 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 02:04:02 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parsing.h"

int	is_slash(const char *s1)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] == '/')
			return (1);
		else
			i++;
	}
	return (0);
}

int	env_is_alive(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strcmp(envp[i], "PATH=") == 0)
			return (0);
		i++;
	}
	return (1);
}

void	ft_replace_tab(t_parse **node)
{
	char	*tmp;

	tmp = ft_strdup((*node)->data->content[0]);
	if (!tmp)
	{
		free
	}
}

void	is_command_whithout_env(t_parse **node, char **envp)
{
	if (is_slash((*node)->data->content) || env_is_alive(envp))
	{
		if (!parse_path_whitout_env((*node)->data->content[0]))
		{
			if (!(*node)->data->content[0])
			{
				free_parse(*node);
				ft_error("Malloc failed in function 'parse_path_without_env'", MEM_ALLOC);
			}
			return ;
		}
		else
		{
			ft_replace_tab(node);
			(*node)->token = CMD;
			return ;
		}
	}
}

void	is_command(t_parse **node, char **envp)
{
	char	*tmp;
	
	tmp = extract_path(envp);
		if (!tmp)
		{
			free_parse(*node);
			ft_error("Malloc failed in function 'is_command'", MEM_ALLOC);
		}
		path = split_the_path(tmp);
		free(tmp);
		if (!path)
		{
			free_parse(*node);
			ft_error("Malloc failed in function 'is_command'", MEM_ALLOC);
		}
		cmd_path = extract_cmd(arg, path);
		if (!cmd_path)
		{
			free_parse(*node);
			ft_error("Malloc failed in function 'is_command'", MEM_ALLOC);
		}
}
