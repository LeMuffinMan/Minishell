/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_path_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:53:37 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:12:52 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "list.h"
#include "structs.h"
#include "errno.h"
#include <unistd.h>

bool	env_is_alive(t_var *list_env)
{
	t_var	*tmp;

	if (!list_env)
		return (false);
	tmp = list_env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "PATH", 5))
			return (1);
		tmp = tmp->next;
	}
	return (false);
}

static void	case_of_absolute_path(t_token **node)
{
	char	*tmp;

	tmp = ft_strdup((*node)->content[0]);
	if (!tmp)
	{
		free_parse(*node);
		errno = MEM_ALLOC;
		return ;
	}
	free_tab((*node)->content);
	(*node)->content = ft_split(tmp, ' ');
	if (!(*node)->content || !(*node)->content[0])
	{
		free_parse(*node);
		free(tmp);
		errno = MEM_ALLOC;
		return ;
	}
	free(tmp);
}

void	alloc_cmd_split(char ***split_cmd, char **path, char *arg, int *error)
{
	errno = SUCCESS;
	*split_cmd = ft_split(arg, ' ');
	if (!*split_cmd || !*split_cmd[0])
	{
		free_tab(path);
		if (*split_cmd && !*split_cmd[0])
		{
			free_tab(*split_cmd);
			*error = CMD_NOT_FOUND;
		}
		if (errno)
			errno = MEM_ALLOC;
	}
}

void	replace_tab(t_token **node, char *str)
{
	char	*tmp;

	if (!str)
		case_of_absolute_path(node);
	else
	{
		tmp = ft_strdup((*node)->content[0]);
		if (!tmp)
		{
			free_parse(*node);
			errno = MEM_ALLOC;
			return ;
		}
		free_tab((*node)->content);
		(*node)->content = ft_split(str, ' ');
		if (!(*node)->content || !(*node)->content[0])
		{
			free_parse(*node);
			errno = MEM_ALLOC;
		}
		free(tmp);
	}
}

bool	verif_access_exec(char *tmp_cmd, int *error)
{
	int	i;

	i = access(tmp_cmd, F_OK);
	if (i == 0)
	{
		i = access(tmp_cmd, X_OK);
		if (i != 0)
		{
			*error = PERMISSION_DENIED;
			return (false);
		}
	}
	else
	{
		*error = CMD_NOT_FOUND;
		return (false);
	}
	*error = SUCCESS;
	return (true);
}
