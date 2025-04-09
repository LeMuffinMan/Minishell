/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:51:51 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 04:49:58 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "stack.h"
#include "libft.h"
#include <stdlib.h>

char	*verif_command(t_parse **node, char *tmp, char **path, char **envp)
{
	tmp = extract_path(envp);
	if (!tmp)
		free_parse(*node, "Malloc failed in function 'is_command'", MEM_ALLOC);
	path = split_the_path(tmp);
	free(tmp);
	tmp = NULL;
	if (!path)
		free_parse(*node,
			"Malloc failed in function 'split_the_path'", MEM_ALLOC);
	tmp = parse_cmd((*node)->content[0], path, &(*node)->error);
	if ((*node)->error == CMD_NOT_FOUND || (*node)->error == PERMISSION_DENIED)
		return (NULL);
	if (!tmp)
		free_parse(*node, "Malloc failed in function 'parse_cmd'", MEM_ALLOC);
	return (tmp);
}

void	is_built_in(t_parse **node)
{
	if (ft_strncmp((*node)->content[0], "echo", 4) == 0
		&& (*node)->content[0][4] == ' ')
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "cd", 2) == 0
		&& (*node)->content[0][2] == ' ')
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "pwd", 3) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "export", 6) == 0
		&& (*node)->content[0][6] == ' ')
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "unset", 5) == 0
		&& (*node)->content[0][5] == ' ')
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "env", 3) == 0
		&& (*node)->content[0][3] == ' ')
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "exit", 4) == 0
		&& (*node)->content[0][4] == ' ')
		(*node)->token = BUILT_IN;
}
