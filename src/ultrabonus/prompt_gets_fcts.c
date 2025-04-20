/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_gets_fcts.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:39:03 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:45:30 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "get_prompt.h"
#include "libft.h"
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

char *get_ps1(t_var **env)
{
  t_var *tmp;

  tmp = *env;
  while (tmp)
  {
    if (!ft_strncmp(tmp->value, "PS1", 4))
      return (tmp->value);
    tmp = tmp->next;
  }
  return (NULL);
}

char *get_user()
{
  DIR *d;
  struct dirent *dir;
  char *username;
  char *path;

  username = NULL;
  d = opendir("/home");
  if (!d)
    return (NULL);
  while ((dir = readdir(d)) != NULL)
  {
    if (ft_strncmp(dir->d_name, ".", 2) == 0 || ft_strncmp(dir->d_name, "..", 3) == 0)
      continue;
    path = ft_strjoin("/home/", dir->d_name);
    if (access(path, F_OK | X_OK) == 0)
    {
      if (!username) 
        username = ft_strdup(dir->d_name);
      else 
      {
        free(path);
        closedir(d);
        return (NULL);
      }
    }
    free(path);
  }
  closedir(d);
  return (username);
}

char *get_hostname(int mode)
{
  char *hostname;
  char buffer[256];
  int fd;
  ssize_t bytes_read;

  fd = open("/etc/hostname", O_RDONLY);
  if (fd == -1) //retour d'erreur a refaire 
      return NULL;
  bytes_read = read(fd, buffer, sizeof(buffer) - 1);
  close(fd);
  if (bytes_read <= 0)
      return NULL;
  buffer[bytes_read] = '\0';
  hostname = trim_hostname(buffer, mode);
  return (hostname);
}

char *get_uidline()
{
  int fd;
  char buffer[4096];
  ssize_t bytes_read;
  char *uid_line = NULL;
  char *line_start;
  char *line_end;

  fd = open("/proc/self/status", O_RDONLY);
  if (fd == -1)
      return NULL;
  bytes_read = read(fd, buffer, sizeof(buffer) - 1);
  close(fd);
  if (bytes_read <= 0)
      return NULL;
  buffer[bytes_read] = '\0';
  line_start = buffer;
  line_end = ft_strchr(line_start, '\n');
  while (line_end)
  {
      *line_end = '\0';
      if (ft_strncmp(line_start, "Uid:", 4) == 0)
      {
          uid_line = ft_strdup(line_start);
          break;
      }
      line_start = line_end + 1;
    line_end = ft_strchr(line_start, '\n');
  }
  return (uid_line);
}

char *ft_getuid()
{
  char *line;
  char *uid;
  int i;
  int j;

  uid = NULL;
  line = get_uidline();
  if (line && !ft_strncmp("Uid:", line, 4))
  {
    i = 4;
    while (line[i] && !ft_isdigit(line[i]))
        i++;
    j = i;
    while (line[j] && ft_isdigit(line[j]))
        j++;
    uid = ft_substr(line, i, j - i);
  }
  free(line);
  return (uid);
}
