/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_lexer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:30:14 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/14 18:31:04 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include <stdio.h>

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
