/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:59:29 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/05 00:10:55 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "here_doc.h"
#include "libft.h"

bool	verif_name(char *name)
{
	struct stat	status;

	if (stat(name, &status) == 0)
	{
		free(name);
		return (false);
	}
	return (true);
}

bool	free_lim_close_fd(char *limiter, int fd)
{
	free(limiter);
	if (close(fd) == -1)
		return (false);
	return (true);
}

char	*get_limiter(char *s, int fd)
{
	char	*limiter;
	char *tmp;

	limiter = ft_strdup(s);
	if (!limiter)
	{
		close(fd);
		return (NULL);
	}
	tmp = limiter;
	limiter = ft_strjoin(limiter, "\n");
	free(tmp);
	return (limiter);
}

bool	case_is_limiter(char *line, char *limiter, int fd, int len)
{
	if (!ft_strncmp(line, limiter, len))
	{
		free(line);
		return (true);
	}
	if (write(fd, line, ft_strlen(line)) == -1)
	{
		free(line);
		return (false);
	}
	return (false);//passe de true a false pour fix here doc qui se ferme a a 1ere ligne
}

bool	verif_here_doc(t_token **head)
{
	if (!head || !*head)
		return (true);
	if (!create_hd_name(head))
	{
		free_parse(*head);
		errno = MEM_ALLOC;
		return (false);
	}
	return (true);
}
