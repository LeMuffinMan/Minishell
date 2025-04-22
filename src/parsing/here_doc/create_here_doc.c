/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:50:55 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/22 05:15:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "list.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include "ft_printf.h"

static char	*make_hd_name(int fd, char *print_char, char *res)
{
	char	*tmp;

	tmp = NULL;
	while (ft_strlen(res) < NAME_LENGTH)
	{
		read(fd, print_char, 1);
		print_char[1] = '\0';
		if (ft_isprint(print_char[0]))
		{
			tmp = ft_strjoin(res, print_char);
			if (!tmp)
			{
				free(res);
				return (NULL);
			}
			free(res);
			res = ft_strdup(tmp);
			if (!res)
			{
				free(tmp);
				return (NULL);
			}
		}
	}
	return (res);
}

static bool	replace_content(t_token	**node, char *add_content)
{
	char	**res;
	
	res = malloc(sizeof(char *) * 4);
	if (!res)
		return (false);
	res[0] = ft_strdup((*node)->content[0]);
	if (!res[0])
	{
		free_tab(res);
		return(false);
	}
	res[1] = ft_strdup((*node)->content[1]);
	if (!res[1])
	{
		free_tab(res);
		return (false);
	}
	res[2] = ft_strdup(add_content);
	if (!res[2])
	{
		free_tab(res);
		return (false);
	}
	res[3] = NULL;
	free_tab((*node)->content);
	(*node)->content = res;
	return (true);
}

static bool	handle_name(t_token **node)
{
	char	*name;
	char	*print_char;
	char	*res;
	int		fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (false);
	res = ft_calloc(1, 1);
	if (!res)
	{
		close(fd);
		return (false);
	}
	print_char = ft_calloc(2, 1);
	name = make_hd_name(fd, print_char, res);
	free(print_char);
	close(fd);
	if (!name)
		return (false);
	if (!replace_content(node, name))
	{
		free(name);
		return (false);
	}
	free(name);
	return (true);
}

bool	handle_here_doc(t_token **head)
{
	t_token	*tmp;

	if (!*head)
		return (false);
	tmp = *head;
	while (tmp)
	{
		if (tmp->token == HD)
		{
			if (!handle_name(&tmp))
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}
