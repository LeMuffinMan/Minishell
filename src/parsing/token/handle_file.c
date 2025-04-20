/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:38:32 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/20 16:49:07 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

static bool	is_file(t_token **node)
{
	
}

t_token	*handle_file(t_token **head)
{
	t_token	*tmp;

	if (!*head)
		return ;
	tmp = *head;
	while (tmp)
	{
		if (is_file(tmp))
		{
			tmp->token = F_OR_D;
		}
	}
}