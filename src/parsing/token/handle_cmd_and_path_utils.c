/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_path_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:53:37 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/22 19:16:49 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "list.h"
#include <unistd.h>

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
			return (1);
		i++;
	}
	return (0);
}

static void	case_of_absolute_path(t_token **node)
{
	char	*tmp;

	tmp = ft_strdup((*node)->content[0]);
	if (!tmp)
		free_parse(*node, "Malloc failed in 'case_of_absolute_path'",
			MEM_ALLOC);
	free_tab((*node)->content);
	(*node)->content = ft_split(tmp, ' ');
	if (!(*node)->content || !(*node)->content[0])
	{
		free(tmp);
		free_parse(*node, "Malloc failed in 'case_of_absolute_path'",
			MEM_ALLOC);
	}
	free(tmp);
}

/* int print_tab(char **s)
{
	int i;

	#include <stdio.h>
	printf("%s\n", s[0]);
	i = 0;
	while (s[i])
	{
		#include <stdio.h>
		printf("%s\n", s[i]);
		i++;
	}
	return (0);
} */

void	replace_tab(t_token **node, char *str)
{
	char	*tmp;

	/* #include <stdio.h> */
	/* printf("%s\n", str); */
	if (!str)
		case_of_absolute_path(node);
	else
	{
		tmp = ft_strdup((*node)->content[0]);
		if (!tmp)
			free_parse(*node, "Malloc failed in 'replace_tab'", MEM_ALLOC);
		free_tab((*node)->content);
		(*node)->content = ft_split(str, ' ');
		// print_tab((*node)->content);
		if (!(*node)->content || !(*node)->content[0])
		{
			free(tmp);
			free_parse(*node, "Malloc failed in 'replace_tab'", MEM_ALLOC);
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
