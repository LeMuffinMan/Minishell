/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:58:17 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 18:00:33 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_error_handlers.h"
#include "structs.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void	malloc_error_freelists_exit(t_lists *lists)
{
	int	saved_errno;

	saved_errno = errno;
	free_lists(lists);
	exit(saved_errno);
}

int	malloc_error_close_free_pipes(int pipefd[2], t_pipe **pipes)
{
	int	saved_errno;

	saved_errno = errno;
	if (close(pipefd[0]) == -1 || close(pipefd[1]) == -1)
	{
		free_pipes(pipes);
		return (saved_errno);
	}
	free_pipes(pipes);
	return (saved_errno);
}
