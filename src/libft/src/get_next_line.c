/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:26:29 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 00:30:09 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"
#include <unistd.h>

char	*clear_string(char *str, int i)
{
	while (str[i])
	{
		str[i] = '\0';
		i++;
	}
	return (str);
}

char	*buf_is_empty(char *buffer, char *storage, int fd, int read_size)
{
	while (!ft_isnewline(storage) && read_size != 0)
	{
		read_size = read(fd, buffer, BUFFER_SIZE);
		if ((buffer[0] == '\0' && read_size == 0) || (read_size == -1))
		{
			clear_string(buffer, 0);
			free(storage);
			return (NULL);
		}
		buffer[read_size] = '\0';
		storage = ft_strjoin_and_free(storage, buffer);
		if (!storage)
		{
			free(storage);
			return (NULL);
		}
		if ((storage && ft_isnewline(storage))
			|| (storage[1] == '\0' && read_size > 1))
			break ;
	}
	new_memmove(buffer, storage);
	return (storage);
}

char	*buf_is_full(char *buffer, char *storage, int fd, int read_size)
{
	while (!ft_isnewline(storage) && read_size != 0)
	{
		read_size = read(fd, buffer, BUFFER_SIZE);
		if ((buffer[0] == '\0' && read_size == 0) || (read_size == -1))
		{
			clear_string(buffer, 0);
			free(storage);
			return (NULL);
		}
		if (read_size < BUFFER_SIZE)
			buffer = clear_string(buffer, read_size);
		storage = ft_strjoin_and_free(storage, buffer);
		if (!storage)
		{
			free(storage);
			return (NULL);
		}
		if ((storage && ft_isnewline(storage)) || (storage[1] == '\0'))
			break ;
	}
	new_memmove(buffer, storage);
	return (storage);
}

char	*ft_setline(char *line)
{
	int		i;
	char	*str;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	str = copy_and_backspace(line, str, i);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	buffer[FD_MAX + 1][BUFFER_SIZE + 1];
	char		*line;
	char		*storage;
	int			read_size;

	read_size = 1;
	storage = ft_strdup(buffer[fd]);
	if (!storage)
		return (NULL);
	if (fd < 0 || fd > FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd][0])
		line = buf_is_empty(buffer[fd], storage, fd, read_size);
	else
		line = buf_is_full(buffer[fd], storage, fd, read_size);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	line = ft_setline(line);
	return (line);
}
