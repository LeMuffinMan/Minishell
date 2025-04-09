/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 01:03:28 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "stack.h"
#include "libft.h"
#include "ft_printf.h"

void	is_operand_or_quote(t_parse **node)
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

void	is_redirection(t_parse **node)
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

void	is_command_whithout_env(t_parse **node, char **envp)
{
	if (is_slash((*node)->content[0]) || !env_is_alive(envp))
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

void	is_command(t_parse **node, char **envp)
{
	char	*tmp;
	char	**path;
	char	*cmd_w_path;

	tmp = NULL;
	path = NULL;
	cmd_w_path = NULL;
	is_built_in(node);
	if ((*node)->token == BUILT_IN)
		(*node)->error = SUCCESS;
	if ((int)(*node)->token < 0)
		cmd_w_path = verif_command(node, tmp, path, envp);
	if ((*node)->error == SUCCESS)
	{
		replace_tab(node, cmd_w_path);
		if ((int)(*node)->token < 0)
			(*node)->token = CMD;
	}
	free(cmd_w_path);
}

void	assign_token(t_parse **head, char **envp)
{
	t_parse	*tmp;

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
