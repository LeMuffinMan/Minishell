/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:51:51 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 23:36:10 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "expand.h"
#include "quote.h"
#include <stdlib.h>
#include <errno.h>

void	del_last_space_for_arg(t_token **node, char **tmp)
{
	int		len;
	int		i;

	i = 0;
	len = ft_strlen((*node)->content[0]);
	if ((*node)->content[0][len - 1] == ' ')
	{
		while ((*node)->content[0][i] && (*node)->content[0][i] != ' ')
			i++;
		*tmp = ft_strndup((*node)->content[0], i);
		if (!*tmp)
		{
			free_parse(*node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
		}
	}
	else
	{
		*tmp = ft_strdup((*node)->content[0]);
		if (!*tmp)
		{
			free_parse(*node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
		}
	}
}

void	handle_cmd(t_token **node, t_var *list_env, bool flag)
{
	if (env_is_alive(list_env))
	{
		if ((*node)->content[0][0] != '\0' || (*node)->token == D_QUOTE
		|| (*node)->token == S_QUOTE
		|| (*node)->token == EXPAND
		|| !(*node)->prev
		|| ((*node)->prev->token != CMD &&
			(*node)->prev->token != BUILT_IN &&
			!((*node)->prev->token == SPACE &&
			(*node)->prev->prev &&
			((*node)->prev->prev->token == CMD ||
			(*node)->prev->prev->token == BUILT_IN))))
		{
			if (flag || (*node)->token == NO_TOKEN
				|| (*node)->token == APPEND
				|| (*node)->token == D_QUOTE || (*node)->token == S_QUOTE)
				is_command(node, list_env, flag);
		}
	}
	else
		(*node)->error = PERMISSION_DENIED;
}

static char	*case_of_cmd_quote(t_token *node)
{
	char	*cmd_in_quote;

	cmd_in_quote = NULL;
	if (node->token == D_QUOTE || node->token == S_QUOTE)
	{
		cmd_in_quote = ft_strdup(node->content[0]);
		if (!cmd_in_quote)
		{
			free_parse(node, NULL, MEM_ALLOC);
			errno = MEM_ALLOC;
			return (NULL);
		}
	}
	return (cmd_in_quote);
}

char	*verif_command(t_token **node, char *tmp, char **path, t_var *list_env)
{
	char	*cmd_in_quote;

	cmd_in_quote = case_of_cmd_quote(*node);
	tmp = extract_path(list_env);
	path = split_the_path(tmp);
	if (!path)
	{
		free_parse(*node, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
		return (NULL);
	}
	if ((*node)->token == D_QUOTE || (*node)->token == S_QUOTE)
		tmp = parse_cmd(cmd_in_quote, path, &(*node)->error, false);
	else
		tmp = parse_cmd((*node)->content[0], path, &(*node)->error, true);
	if (cmd_in_quote)
		free(cmd_in_quote);
	if ((*node)->error == CMD_NOT_FOUND || (*node)->error == PERMISSION_DENIED)
		return (NULL);
	if (!tmp)
	{
		free_parse(*node, NULL, MEM_ALLOC);
		errno = MEM_ALLOC;
	}
	return (tmp);
}

void	is_built_in(t_token **node)
{
	if (ft_strncmp((*node)->content[0], "echo", 5) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "cd", 3) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "pwd", 4) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "export", 7) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "unset", 6) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "env", 4) == 0)
		(*node)->token = BUILT_IN;
	else if (ft_strncmp((*node)->content[0], "exit", 5) == 0)
		(*node)->token = BUILT_IN;
	if ((*node)->token == BUILT_IN)
	{
		if ((*node)->curr_dir)
			free((*node)->curr_dir);
	}
}
