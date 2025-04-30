/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/30 23:31:35 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "ft_printf.h"

static void	is_operand_or_quote(t_token **node, t_var *list_env)
{
	is_quote(node, list_env);
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

static void	is_command_whithout_env(t_token **node, char **envp)
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

static void	is_command(t_token **node, char **envp)
{
	char	*tmp;
	char	**path;
	char	*cmd_w_path;

	tmp = NULL;
	path = NULL;
	cmd_w_path = NULL;
	if ((*node)->token == NO_TOKEN
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

void	assign_token(t_token **head, char **envp, t_var *list_env)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		is_redirection(&tmp);
		if (tmp->token == NO_TOKEN)
			is_operand_or_quote(&tmp, list_env);
		if (tmp->token == NO_TOKEN
			|| tmp->token == D_QUOTE || tmp->token == S_QUOTE)
		{
			if (tmp->token == D_QUOTE || tmp->token == S_QUOTE
				|| !tmp->prev || (tmp->prev->token != CMD
					&& tmp->prev->token != BUILT_IN))
			{
				is_command_whithout_env(&tmp, envp);
				if (tmp->token == NO_TOKEN
					|| tmp->token == D_QUOTE || tmp->token == S_QUOTE)
					is_command(&tmp, envp);
			}
			else
				tmp->error = PERMISSION_DENIED;
		}
		tmp = tmp->next;
	}
}
