/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:03:53 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:16:12 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"
#include "list.h"
#include "libft.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

static bool	make_wildcard(t_token **node, bool flag)
{
	char	**current_dir;
	int		sort_of;

	current_dir = extract_current_dir();
	if (errno == MEM_ALLOC)
		return (false);
	sort_of = which_case_of_wildcard((*node)->content[0]);
	if (sort_of == INFIX)
		is_infix(node, current_dir);
	else if (sort_of == PREFIX)
		is_prefix(node, current_dir);
	else if (sort_of == SUFFIX)
		is_suffix(node, current_dir);
	else
		is_joker(node, current_dir);
	free_tab(current_dir);
	if (errno == MEM_ALLOC)
		return (false);
	sort_wildcard_arg((*node)->content);
	if (flag)
		add_space_for_wildcard(node);
	if (errno == MEM_ALLOC)
		return (false);
	return (true);
}

static bool	is_valid_arg_for_echo(t_type token)
{
	if (token == R_IN || token == HD || token == APPEND
		|| token == TRUNC || token == PIPE || token == O_OR
		|| token == O_AND || token == CMD)
		return (true);
	return (false);
}

static bool	is_an_echo_arg(t_token *node)
{
	bool	flag;

	flag = false;
	while (node && !is_valid_arg_for_echo(node->token))
	{
		if (node
			&& !ft_strncmp(node->content[0], "echo", 5))
			flag = true;
		node = node->prev;
	}
	return (flag);
}

static bool	wildcard_processing(t_token **node)
{
	bool	is_echo;

	is_echo = false;
	if ((*node)->token == WILDCARD
		|| ((*node)->token != S_QUOTE
			&& is_wildcard((*node)->content[0])))
	{
		if (is_an_echo_arg(*node))
			is_echo = true;
		if (!make_wildcard(node, is_echo))
		{
			free_parse(*node);
			errno = MEM_ALLOC;
			return (false);
		}
	}
	return (true);
}

bool	handle_wildcard(t_token **head, bool flag)
{
	t_token	*tmp;

	if (!head || !*head || !flag)
		return (true);
	tmp = *head;
	while (tmp)
	{
		if (!wildcard_processing(&tmp))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}
