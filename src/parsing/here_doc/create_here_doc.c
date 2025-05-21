/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:50:55 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 15:24:56 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "list.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include "ft_printf.h"

static bool	free_and_return(char *str, char **tab, int fd)
{
	if (str)
		free(str);
	if (tab)
		free_tab(tab);
	if (fd > 0)
		close(fd);
	return (false);
}

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
			free(tmp);
			if (!res)
				return (NULL);
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
		return (free_and_return(NULL, res, -1));
	res[1] = ft_strdup((*node)->content[1]);
	if (!res[1])
		return (free_and_return(NULL, res, -1));
	res[2] = ft_strdup(add_content);
	if (!res[2])
		return (free_and_return(NULL, res, -1));
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
		return (free_and_return(NULL, NULL, fd));
	print_char = ft_calloc(2, 1);
	if (!print_char)
		return (free_and_return(res, NULL, fd));
	name = make_hd_name(fd, print_char, res);
	free(print_char);
	close(fd);
	if (!name)
		return (false);
	if (!replace_content(node, name))
		return (free_and_return(name, NULL, -1));
	free(name);
	return (true);
}

bool	create_hd_name(t_token **head)
{
	t_token	*tmp;

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
