/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/08 15:48:58 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "ft_printf.h"

static void	is_operand_or_quote(t_token **node)
{
	is_quote(node);
	if (!ft_strcmp((*node)->content[0], "&&"))
		(*node)->token = O_AND;
	else if (!ft_strcmp((*node)->content[0], "||"))
		(*node)->token = O_OR;
	else if (!ft_strcmp((*node)->content[0], "|"))
		(*node)->token = PIPE;
	else if (!ft_strcmp((*node)->content[0], "*"))
		(*node)->token = WILDCARD;
	else if ((*node)->content[0][0] == '(')
		(*node)->token = L_PARENTHESIS;
	else if ((*node)->content[0][0] == ')')
		(*node)->token = R_PARENTHESIS;
	else if (ft_str_isspace((*node)->content[0]))
		(*node)->token = SPACE;
}

static void	is_redirection(t_token **node)
{
	if (!ft_strcmp((*node)->content[0], ">>"))
		(*node)->token = APPEND;
	else if (!ft_strcmp((*node)->content[0], "<<"))
		(*node)->token = HD;
	else if (!ft_strcmp((*node)->content[0], "<"))
		(*node)->token = R_IN;
	else if (!ft_strcmp((*node)->content[0], ">"))
		(*node)->token = TRUNC;
}

void	is_command_whithout_env(t_token **node, char **envp)
{
	is_built_in(node);
	if ((*node)->token == BUILT_IN)
		(*node)->error = SUCCESS;
	else if (ft_isslash((*node)->content[0]) || !env_is_alive(envp))
	{
		if (!parse_path_without_env(*node))
			return ;
		else
		{
			replace_tab(node, NULL);
			(*node)->error = SUCCESS;
			(*node)->token = CMD;
		}
	}
}

void	is_command(t_token **node, char **envp)
{
	char	*tmp;
	char	**path;
	char	*cmd_w_path;

	tmp = NULL;
	path = NULL;
	cmd_w_path = NULL;
	if ((*node)->token == NO_TOKEN || (*node)->token == EXPAND
		|| (*node)->token == D_QUOTE || (*node)->token == S_QUOTE)
		cmd_w_path = verif_command(node, tmp, path, envp);
	if (cmd_w_path && ((*node)->error == SUCCESS || (*node)->error == QUOTE))
	{
		if ((*node)->prev && ((*node)->prev->token == R_IN
				|| (*node)->prev->token == HD
				|| (*node)->prev->token == APPEND
				|| (*node)->prev->token == TRUNC))
		{
			free(cmd_w_path);
			return ;
		}
		replace_tab(node, cmd_w_path); //exec : fix l'absence de path
		(*node)->token = CMD;
	}
	free(cmd_w_path);
}

void	assign_token(t_token **head, char **envp, t_var *list_env, bool flag)
{
	t_token	*tmp;

	tmp = *head;
	(void)list_env;
	while (tmp)
	{
		is_redirection(&tmp);
		if (tmp->token == NO_TOKEN)
			is_operand_or_quote(&tmp);
		if (tmp->token == NO_TOKEN
			|| tmp->token == D_QUOTE || tmp->token == S_QUOTE
			|| tmp->token == EXPAND)
			handle_cmd(&tmp, envp, flag);
		tmp = tmp->next;
	}
}
