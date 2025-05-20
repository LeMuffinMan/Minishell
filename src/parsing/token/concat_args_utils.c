/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:44:43 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/20 19:33:28 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "quote.h"
#include <stdlib.h>

int	is_valid_prev(t_token *prev)
{
	if (!prev)
		return (1);
	if (prev->token == SPACE)
		return (is_valid_prev(prev->prev));
	if ((prev->token == PIPE
			|| prev->token == O_OR
			|| prev->token == O_AND
			|| prev->token == R_PARENTHESIS
			|| prev->token == L_PARENTHESIS
			|| prev->token == DIREC
			|| prev->token == FLE))
		return (1);
	return (0);
}

void	handle_is_command(t_token *node, char *cmd_w_path, bool flag)
{
	if (cmd_w_path && (node->error == SUCCESS || node->error == QUOTE))
	{
		if (node->prev && (node->prev->token == R_IN
				|| node->prev->token == HD
				|| node->prev->token == APPEND
				|| node->prev->token == TRUNC))
		{
			free(cmd_w_path);
			return ;
		}
		if (!is_valid_prev(node->prev))
		{
			node->error = CMD_NOT_FOUND;
			return ;
		}
		if (flag)
			replace_tab(&node, cmd_w_path); //exec : fix l'absence de path
		node->token = CMD;
	}
}

static void	case_of_directory_error(t_token **node)
{
	int	len;

	len = ft_strlen((*node)->content[0]);
	if ((*node)->content[0][len - 1] == '/')
		(*node)->error = IS_A_DIR;
	else
		(*node)->error = CMD_NOT_FOUND;
}

static t_token	*set_syntax_error(t_token *node)
{
	t_token	*head;
	char	*operator;
	char	*tmp;
	char	*new_content;

	if (!node || !node->content || !node->content[0])
		return (NULL);
	operator = ft_strdup(node->content[0]);
	if (!operator)
		return (NULL);
	free_parse(node, NULL, 0);
	tmp = ft_strjoin("syntax error near unexpected token `", operator);
	free(operator);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "'");
	free(tmp);
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, NO_F_OR_D);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}

static t_token	*set_parenthesis_error(t_token *node)
{
	t_token	*head;
	char	*new_content;

	if (!node || !node->content || !node->content[0])
		return (NULL);
	free_parse(node, NULL, 0);
	new_content = ft_strdup("Usage: Use parenthesis for priorities of '&&' and '||'");
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, E_IO);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}

bool	syntax_error_for_op(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->token == O_OR || tmp->token == O_AND)
		{
			if (!tmp->prev || !tmp->next
				|| (tmp->prev->token != CMD && tmp->prev->token != BUILT_IN
					&& tmp->prev->token != R_PARENTHESIS && tmp->prev->error == CMD_NOT_FOUND 
					&& tmp->prev->error == PERMISSION_DENIED)
				|| (tmp->next->token != CMD && tmp->next->token != BUILT_IN
					&& tmp->next->token != L_PARENTHESIS))
			{
				*head = set_syntax_error(tmp);
				return (true);
			}
		}
		if (tmp->token == L_PARENTHESIS && (tmp->prev
			&& tmp->prev->token != O_AND && tmp->prev->token != O_OR && tmp->next->token != L_PARENTHESIS))
		{
			*head = set_parenthesis_error(tmp);
			return (true);
		}
		if (tmp->token == R_PARENTHESIS && (tmp->next
			&& tmp->next->token != O_AND && tmp->next->token != O_OR && tmp->next->token != R_PARENTHESIS))
		{
			*head = set_parenthesis_error(tmp);
			return (true);
		}
		if (tmp->token == PIPE)
		{
			if (!tmp->prev || !tmp->next
				|| (tmp->prev->token != CMD && tmp->prev->token != BUILT_IN)
				|| (tmp->next->token != CMD && tmp->next->token != BUILT_IN))
			{
				*head = set_syntax_error(tmp);
				return (true);
			}
		}
		tmp = tmp->next;
	}
	return (false);
}

static t_token	*set_quote_or_par_error(t_token *node, t_type token)
{
	t_token	*head;
	char	*operator;
	char	*tmp;
	char	*new_content;

	if (!node || !node->content)
		return (NULL);
	if (token == S_QUOTE)
		operator = ft_strdup("'");
	else if (token == D_QUOTE)
		operator = ft_strdup("\"");
	else if (token == L_PARENTHESIS)
		operator = ft_strdup("(");
	else
		operator = ft_strdup(")");
	if (!operator)
		return (NULL);
	free_parse(node, NULL, 0);
	tmp = ft_strjoin("syntax error near unexpected token `", operator);
	free(operator);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "'");
	free(tmp);
	if (!new_content)
		return (NULL);
	head = add_new_token(new_content, NO_F_OR_D);
	free(new_content);
	if (!head)
		return (NULL);
	return (head);
}

void	error_one_quote(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (tmp->error == PB_QUOTE)
		{
			*head = set_quote_or_par_error(tmp, tmp->token);
			return ;
		}
		tmp = tmp->next;
	}
}

void	error_one_parenthesis(t_token **head)
{
	t_token	*tmp;
	int		left;
	int		right;

	if (!head || !*head)
		return ;
	tmp = *head;
	left = 0;
	right = 0;
	while (tmp)
	{
		if (tmp->token == L_PARENTHESIS)
			left++;
		else if (tmp->token == R_PARENTHESIS)
			right++;
		tmp = tmp->next;
	}
	if (left > right)
		*head = set_quote_or_par_error(*head, R_PARENTHESIS);
	else if (left < right)
		*head = set_quote_or_par_error(*head, L_PARENTHESIS);
}

void	check_syntax_error(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	if (syntax_error_for_op(head))
		return ;
	while (tmp)
	{
		if ((((tmp->token == R_IN) || (tmp->token == HD)
					|| (tmp->token == TRUNC) || (tmp->token == APPEND))
				&& !tmp->content[1]))
		{
			*head = set_syntax_error(tmp);
			return ;
		}
		else if (tmp->token == DIREC)
		{
			case_of_directory_error(&tmp);
			return ;
		}
			tmp = tmp->next;
	}
}
