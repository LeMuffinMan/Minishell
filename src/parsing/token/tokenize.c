/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 23:28:45 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "ft_printf.h"
#include "quote.h"
#include "wildcard.h"
#include <sys/stat.h>
#include <errno.h>

static void	is_operand_or_quote(t_token **node)
{
	if (is_quote(node))
		return ;
	if (!ft_strcmp((*node)->content[0], "&&"))
		(*node)->token = O_AND;
	else if (!ft_strcmp((*node)->content[0], "||"))
		(*node)->token = O_OR;
	else if (!ft_strcmp((*node)->content[0], "|"))
		(*node)->token = PIPE;
	else if (is_wildcard((*node)->content[0]))
		(*node)->token = WILDCARD;
	else if ((*node)->content[0][0] == '(')
		(*node)->token = L_PARENTHESIS;
	else if ((*node)->content[0][0] == ')')
		(*node)->token = R_PARENTHESIS;
	else if (ft_str_isspace((*node)->content[0]))
		(*node)->token = SPACE;
}

static void	is_redirection_or_f_or_d(t_token **node, t_var *list_env)
{
	struct stat	status;
	char		*tmp;

	del_last_space_for_arg(node, &tmp);
	if (!ft_strcmp((*node)->content[0], ">>"))
		(*node)->token = APPEND;
	else if (!ft_strcmp((*node)->content[0], "<<"))
		(*node)->token = HD;
	else if (!ft_strcmp((*node)->content[0], "<"))
		(*node)->token = R_IN;
	else if (!ft_strcmp((*node)->content[0], ">"))
		(*node)->token = TRUNC;
	else if (is_pwd_valid(list_env))
	{
		if (stat(tmp, &status) == 0)
		{
			if (S_ISREG(status.st_mode))
				(*node)->token = FLE;
			if (S_ISDIR(status.st_mode))
				(*node)->token = DIREC;
		}
	}
	free(tmp);
}

void	is_command_whithout_env(t_token **node, t_var *list_env)
{
	is_built_in(node);
	if ((*node)->token == BUILT_IN)
		(*node)->error = SUCCESS;
	if (!is_valid_prev((*node)->prev))
		return ;
	else if (ft_isslash((*node)->content[0]) || !env_is_alive(list_env))
	{
		if (!parse_path_without_env(*node))
			return ;
		else
		{
			replace_tab(node, NULL);
			if ((*node)->error != PERMISSION_DENIED)
				(*node)->error = SUCCESS;
			(*node)->token = CMD;
		}
	}
}

void	is_command(t_token **node, t_var *list_env, bool flag)
{
	char	*tmp;
	char	**path;
	char	*cmd_w_path;
	bool	is_lit_expand;

	tmp = NULL;
	path = NULL;
	cmd_w_path = NULL;
	is_lit_expand = false;
	if (!(*node)->content[0][0])
		return ;
	if ((*node)->error == LITERAL_EXPAND)
		is_lit_expand = true;
	if ((*node)->token == NO_TOKEN || (*node)->token == EXPAND
		|| (*node)->token == D_QUOTE || (*node)->token == S_QUOTE)
		cmd_w_path = verif_command(node, tmp, path, list_env);
	handle_is_command(*node, cmd_w_path, flag);
	free(cmd_w_path);
	if (is_lit_expand && (*node)->error != 0)
		(*node)->error = LITERAL_EXPAND;
}
void	assign_token(t_token **head, t_var *list_env, bool flag)
{
	t_token	*tmp;

	tmp = *head;
	errno = SUCCESS;
	while (tmp && errno != MEM_ALLOC)
	{
		if (tmp->token != ARG)
			is_command_whithout_env(&tmp, list_env);
		if (tmp->token == NO_TOKEN)
			is_redirection_or_f_or_d(&tmp, list_env);
		if (tmp->token == NO_TOKEN)
			is_operand_or_quote(&tmp);
		if (tmp->error == PB_QUOTE)
			return ;
		if (tmp->token == NO_TOKEN
			|| (tmp->token == D_QUOTE && tmp->content[0][0])
			|| (tmp->token == S_QUOTE && tmp->content[0][0])
			|| tmp->token == EXPAND)
			handle_cmd(&tmp, list_env, flag);
		if ((tmp->token == D_QUOTE || tmp->token == D_QUOTE
				|| tmp->error == QUOTE)
			&& is_to_expand(tmp->content[0]))
			tmp->error = LITERAL_EXPAND;
		tmp = tmp->next;
	}
}
