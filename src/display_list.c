/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:15:40 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/09 16:28:16 by asinsard         ###   ########lyon.fr   */
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
			BOLD_BLUE, BOLD_RED, node->error, BOLD_BLUE, STOP_COLOR);
	}
	else
		printf("%sNODE %d: [%s%s%s]--->[%s%s%s]--->[%s%d%s]\n | ^\n v |\n%s",
			BOLD_BLUE, i, BOLD_GREEN, arg[0], BOLD_BLUE,
			BOLD_PURPLE, tok[node->token], BOLD_BLUE,
			BOLD_RED, node->error, BOLD_BLUE, STOP_COLOR);
}

static void	display_unvalid(int i, t_token *node)
{
	printf("%sNODE %d: [%s%s%s]--->[%s%d%s]\n | ^\n v |\n%s",
		BOLD_BLUE, i,
		BOLD_GREEN, node->content[0], BOLD_BLUE,
		BOLD_RED, node->error, BOLD_BLUE, STOP_COLOR);
}

static void	print_token(t_token *head, const char **tab, int i)
{
	printf("%sNULL\n ^\n |\n%s", BOLD_BLUE, STOP_COLOR);
	while (head->next)
	{
		if ((int)head->token != 0)
			display_valid(head, i, head->content, tab);
		else
			display_unvalid(i, head);
		head = head->next;
		i++;
	}
	if ((int)head->token != 0)
		display_valid(head, i, head->content, tab);
	else
		display_unvalid(i, head);
	printf("%sNULL\n%s", BOLD_BLUE, STOP_COLOR);
}

void	display_list(t_token *head, int debug)
{
	int					i;
	static const char	*tab[20] = {"No Token", "Append", "Built_in", 
		"Command", "Directory", "Double quote", "Expand", "File",
		"Group Parenthesis", "Here doc", "And", "Or", "Left Parenthesis", 
		"Right Parenthesis", "Pipe", "Redir in", "Simple quote", "Space",
		"Truncate", "Wildcard"};

	i = 1;
	if (!head)
		return ;
	if (debug)
		print_token(head, tab, i);
}

void	display_lexer(t_lexer *head, int debug)
{
	t_lexer	*tmp;
	int		i;

	i = 1;
	tmp = head;
	if (!tmp)
		return ;
	if (debug)
	{
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
}
