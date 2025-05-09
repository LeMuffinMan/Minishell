/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:54:05 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/09 14:15:00 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "list.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

bool	is_valid_argcmd(t_token *node)
{
	struct stat	status;

	printf("%s\n", node->content[0]);
	if (node->content[0][0] == '\0')
		return (false);
	else if (stat(node->content[0], &status) == -1)
		return (false);
	else if (S_ISREG(status.st_mode)
		|| S_ISDIR(status.st_mode))
		return (false);
	return (true);
}
