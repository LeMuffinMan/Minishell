/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_stdio_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:47:57 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:34:52 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "list.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int	is_a_directory(char *name)
{
	struct stat	file_infos;

	if (stat(name, &file_infos) == -1)
		return (-1);
	if (S_ISDIR(file_infos.st_mode))
		return (1);
	return (0);
}

int	file_check(char *file, t_type type, int error_code)
{
	if (error_code == 127)
		return (error_cmd(file, error_code));
	if (type == R_IN || type == HD)
	{
		if (access(file, F_OK) == -1)
			return (print_error_file_opening(file,
					": No such file or directory\n", 1));
		if (access(file, R_OK) == -1)
			return (print_error_file_opening(file, ": Permission denied\n", 1));
		if (is_a_directory(file))
			return (print_error_file_opening(file, ": Is a directory\n", 1));
	}
	if (type == APPEND || type == TRUNC)
	{
		if (access(file, F_OK) != -1)
		{
			if (access(file, W_OK) == -1)
				return (print_error_file_opening(file, ": Permission denied\n",
						1));
			if (is_a_directory(file))
				return (print_error_file_opening(file, ": Is a directory\n",
						1));
		}
	}
	return (0);
}

int	dup2_and_close(int fd, t_type type)
{
	if (type == R_IN || type == HD)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (-1);
	}
	else if (type == APPEND || type == TRUNC)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (-1);
	}
	if (close(fd) == -1)
		return (-1);
	return (0);
}

int	open_dup2_close(char *file, t_type type)
{
	int	fd;

	if (type == R_IN || type == HD)
		fd = open(file, O_RDONLY);
	else if (type == APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (type == TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = -1;
	if (fd == -1)
		return (-1);
	return (dup2_and_close(fd, type));
}
