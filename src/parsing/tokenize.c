/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:44:17 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/03 01:47:29 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "libft.h"
#include "ft_printf.h"

void	is_operand_or_quote(t_parse **node)
{
	int	len;
	
	len = ft_strlen((*node)->data->content[0]);
	if ((int)(*node)->token < 0)
	{
		if (!ft_strcmp((*node)->data->content[0], "&&"))
			(*node)->token = O_AND;
		else if (!ft_strcmp((*node)->data->content[0], "||"))
			(*node)->token = O_OR;
		else if (!ft_strcmp((*node)->data->content[0], "|"))
			(*node)->token = PIPE;
		else if (!ft_strcmp((*node)->data->content[0], "*"))
			(*node)->token = WILDCARD;
		else if (((*node)->data->content[0][0] == '"')
					&& (*node)->data->content[0][len - 1] == '"')
			(*node)->token = D_QUOTE;
		else if (((*node)->data->content[0][0] == '\'')
					&& (*node)->data->content[0][len - 1] == '\'')
			(*node)->token = S_QUOTE;
	}
}

void	is_redirection(t_parse **node)
{
	if (!ft_strcmp((*node)->data->content[0], ">>"))
		(*node)->token = APPEND;
	else if (!ft_strcmp((*node)->data->content[0], "<<"))
		(*node)->token = HD;
	else if (!ft_strcmp((*node)->data->content[0], "<"))
		(*node)->token = R_IN;
	else if (!ft_strcmp((*node)->data->content[0], ">"))
		(*node)->token = TRUNC;
}

void	assign_token(t_parse **head)
{
	t_parse	*tmp;

	tmp = *head;
	while (tmp->next)
	{
		is_redirection(&tmp);
		if ((int)tmp->token < 0)
			is_operand_or_quote(&tmp);
		// if ((int)tmp->token < 0)
		{
		// is_command_whithout_env(&tmp, envp);
		// if ((int)tmp->token < 0)
			// is_command(&tmp, envp);
		}
		tmp = tmp->next;
	}
	is_redirection(&tmp);
	if ((int)tmp->token < 0)
		is_operand_or_quote(&tmp);
	// if ((int)tmp->token < 0)
	{
		// is_command_whithout_env(&tmp, envp);
		// if ((int)tmp->token < 0)
			// is_command(&tmp, envp);
	}
}
