/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getuid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:28:54 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/12 15:29:15 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

//reecrire avec la doc open raddr etc ...

int	count_subdirectories(const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		statbuf;
	int				count;
	char			full_path[PATH_MAX];
	int				path_len;
	int				name_len;

	count = 0;
	dir = opendir(path);
	if (dir == NULL)
	{
		write(STDERR_FILENO, "Error opening directory\n", 24);
		return (-1);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		path_len = strlen(path);
		name_len = strlen(entry->d_name);
		if (path_len + 1 + name_len >= PATH_MAX)
			continue ;
		strcpy(full_path, path);
		if (path_len > 0 && full_path[path_len - 1] != '/')
			strcat(full_path, "/");
		strcat(full_path, entry->d_name);
		if (stat(full_path, &statbuf) == -1)
			continue ;
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name,
					"..") == 0)
				continue ;
			count++;
		}
	}
	closedir(dir);
	return (count);
}

char	**get_subdirectories(const char *path, int *count)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		statbuf;
	char			**subdirs;
	int				index;
	char			full_path[PATH_MAX];
	int				path_len;
	int				name_len;

	subdirs = NULL;
	index = 0;
	*count = count_subdirectories(path);
	if (*count <= 0)
		return (NULL);
	subdirs = malloc((*count + 1) * sizeof(char *)); // +1 pour le NULL terminal
	if (!subdirs)
	{
		write(STDERR_FILENO, "Memory allocation failed\n", 26);
		return (NULL);
	}
	dir = opendir(path);
	if (dir == NULL)
	{
		write(STDERR_FILENO, "Error opening directory\n", 24);
		free(subdirs);
		return (NULL);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		path_len = strlen(path);
		name_len = strlen(entry->d_name);
		// Vérification pour éviter un buffer overflow
		if (path_len + 1 + name_len >= PATH_MAX)
			continue ;
		// Construction manuelle du chemin
		strcpy(full_path, path);
		if (path_len > 0 && full_path[path_len - 1] != '/')
			strcat(full_path, "/");
		strcat(full_path, entry->d_name);
		if (stat(full_path, &statbuf) == -1)
			continue ;
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name,
					"..") == 0)
				continue ;
			subdirs[index] = malloc(name_len + 1);
			if (!subdirs[index])
			{
				write(STDERR_FILENO, "Memory allocation failed\n", 26);
				for (int j = 0; j < index; j++)
					free(subdirs[j]);
				free(subdirs);
				closedir(dir);
				return (NULL);
			}
			strcpy(subdirs[index], entry->d_name);
			index++;
		}
	}
	subdirs[index] = NULL; // NULL-terminate the array
	closedir(dir);
	return (subdirs);
}

void	free_subdirectories(char **subdirs)
{
	if (!subdirs)
		return ;
	for (int i = 0; subdirs[i]; i++)
		free(subdirs[i]);
	free(subdirs);
}

int	main(void)
{
	int		count;
	char	**subdirs;
	char *full_path;
	int i;

  i = 0;
	subdirs = get_subdirectories("/home", &count);
	if (subdirs)
	{
    while (subdirs[i])
    {
      full_path = ft_strjoin("/home/", subdirs[i]);
      printf("%s\n", subdirs[i]);
      if (access(full_path, R_OK | X_OK) == 0)
        break;
        free(full_path);
      i++;
    }
    printf("HOME found : %s\n", full_path);
    free(full_path);
		free_subdirectories(subdirs);
	}
	return (0);
}
