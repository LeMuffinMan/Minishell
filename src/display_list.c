/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:15:40 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/23 04:07:17 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

static void	display_valid(t_token *node, int i, char **arg, const char **tok)
{
	int	j;

	j = 0;
	if (arg[j + 1])
	{
		printf("%sNODE %d: ", BOLD_BLUE, i);
		while (arg[j])
		{
			printf("[%s%s%s]--->",
				BOLD_GREEN, arg[j], BOLD_BLUE);
			j++;
		}
		printf("[%s%s%s]--->[%s%d%s]\n | ^\n v |\n%s",
			BOLD_PURPLE, tok[node->token],
			BOLD_BLUE, BOLD_RED, node->priority, BOLD_BLUE, STOP_COLOR);
	}
	else
		printf("%sNODE %d: [%s%s%s]--->[%s%s%s]--->[%s%d%s]\n | ^\n v |\n%s",
			BOLD_BLUE, i, BOLD_GREEN, arg[0], BOLD_BLUE,
			BOLD_PURPLE, tok[node->token], BOLD_BLUE,
			BOLD_RED, node->priority, BOLD_BLUE, STOP_COLOR);
}

static void	display_unvalid(int i, t_token *node)
{
	printf("%sNODE %d: [%s%s%s]--->[%s%d%s]\n | ^\n v |\n%s",
		BOLD_BLUE, i,
		BOLD_GREEN, node->content[0], BOLD_BLUE,
		BOLD_RED, node->priority, BOLD_BLUE, STOP_COLOR);
}

void	display_list(t_token *head)
{
	t_token				*tmp;
	int					i;
	static const char	*tab[16] = {"Append", "Built_in", "CMD", "Infile",
		"Double quote", "Here_doc", "Limiter", "And", "Or", "Left_Par", "Right_Par",
		"Pipe", "Redir in", "Simple quote", "Truncate", "Wildcard"};

	i = 1;
	tmp = head;
	if (!tmp)
		return ;
	printf("%sNULL\n ^\n |\n%s", BOLD_BLUE, STOP_COLOR);
	while (tmp->next)
	{
		if ((int)tmp->token >= 0)
			display_valid(tmp, i, tmp->content, tab);
		else
			display_unvalid(i, tmp);
		tmp = tmp->next;
		i++;
	}
	if ((int)tmp->token >= 0)
		display_valid(tmp, i, tmp->content, tab);
	else
		display_unvalid(i, tmp);
	printf("%sNULL\n%s", BOLD_BLUE, STOP_COLOR);
}

void	display_lexer(t_lexer *head)
{
	t_lexer	*tmp;
	int		i;

	i = 1;
	tmp = head;
	if (!tmp)
		return ;
	printf("%sNULL\n ^\n |\n%s", BOLD_BLUE, STOP_COLOR);
	while (tmp->next)
	{
		printf("%sNODE %d: [%s%s%s]\n | ^\n v |\n%s",
			BOLD_BLUE, i,
			BOLD_GREEN, tmp->arg, BOLD_BLUE, STOP_COLOR);
		tmp = tmp->next;
		i++;
	}
	printf("%sNODE %d: [%s%s%s]\n |\n v\n%s",
		BOLD_BLUE, i,
		BOLD_GREEN, tmp->arg, BOLD_BLUE, STOP_COLOR);
	printf("%sNULL\n%s", BOLD_BLUE, STOP_COLOR);
}
