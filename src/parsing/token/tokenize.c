/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/20 18:22:54 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include "libft.h"
#include "ft_printf.h"

static void	is_operand_or_quote(t_token **node)
{
	int	len;

	len = ft_strlen((*node)->content[0]);
	if (!ft_strcmp((*node)->content[0], "&&"))
		(*node)->token = O_AND;
	else if (!ft_strcmp((*node)->content[0], "||"))
		(*node)->token = O_OR;
	else if (!ft_strcmp((*node)->content[0], "|"))
		(*node)->token = PIPE;
	else if (!ft_strcmp((*node)->content[0], "*"))
		(*node)->token = WILDCARD;
	else if (((*node)->content[0][0] == '"')
				&& (*node)->content[0][len - 1] == '"')
		(*node)->token = D_QUOTE;
	else if (((*node)->content[0][0] == '\'')
				&& (*node)->content[0][len - 1] == '\'')
		(*node)->token = S_QUOTE;
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
	else if (is_slash((*node)->content[0]) || !env_is_alive(envp))
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
	if ((int)(*node)->token < 0)
		cmd_w_path = verif_command(node, tmp, path, envp);
	if ((*node)->error == SUCCESS)
	{
		/* replace_tab(node, cmd_w_path); //exec : fix l'absence de path */
		if ((int)(*node)->token < 0)
			(*node)->token = CMD;
	}
	free(cmd_w_path);
}

void	assign_token(t_token **head, char **envp)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp->next)
	{
		is_redirection(&tmp);
		if ((int)tmp->token < 0)
			is_operand_or_quote(&tmp);
		if ((int)tmp->token < 0)
		{
			is_command_whithout_env(&tmp, envp);
			if ((int)tmp->token < 0)
				is_command(&tmp, envp);
		}
		tmp = tmp->next;
	}
	is_redirection(&tmp);
	if ((int)tmp->token < 0)
		is_operand_or_quote(&tmp);
	if ((int)tmp->token < 0)
	{
		is_command_whithout_env(&tmp, envp);
		if ((int)tmp->token < 0)
			is_command(&tmp, envp);
	}
}
