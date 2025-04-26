/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_branch_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:41:11 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:43:52 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

char *exctract_branch(char *path_to_head)
{
  char buffer[256];
  int fd;
  ssize_t bytes_read;
  char *git_branch;
  /* char *tmp; */

  fd = open(path_to_head, O_RDONLY);
  if (fd == -1) //retour d'erreur a refaire 
      return NULL;
  bytes_read = read(fd, buffer, sizeof(buffer) - 1);
  close(fd);
  if (bytes_read <= 0)
      return NULL;
  buffer[bytes_read] = '\0';
  if (!ft_strncmp("ref: refs/heads/", buffer, ft_strlen("ref: refs/heads/")))
  {
    git_branch = ft_strdup(buffer + ft_strlen("ref: refs/heads/"));
    return (git_branch);
  }
  else 
    return (NULL);
}

char *trim_git_branch(char *s)
{
  int i;
  int start;
  int end;
  char *trimmed;

  if (!s)
    return (NULL);
  i = ft_strlen(s);
  while (i >= 0 && s[i] != '/')
    i--;
  start = i + 1;
  end = ft_strlen(s) - 1;
  while (end >= 0 && !ft_isalnum(s[end]))
    end--;
  if (end < start)
    return (ft_strdup(""));
  trimmed = malloc(sizeof(char) * (end - start + 2));
  if (!trimmed)
    return (NULL);
  ft_strlcpy(trimmed, s + start, end - start + 2);
  return (trimmed);
}

char *get_branch(char *pwd)
{
  char *git_branch;
  char *path_to_head;
  char *tmp;

  git_branch = NULL;
  path_to_head = ft_strjoin(pwd, "/.git/HEAD");
  //si on va dans SRC on trouve plus la branche
  if (access(path_to_head, F_OK | R_OK) == 0)
  {
    git_branch = exctract_branch(path_to_head);
    if (git_branch)
    {
      tmp = git_branch;
      git_branch = trim_git_branch(git_branch);
      free(tmp);
    }
  }
  free(path_to_head);
  return (git_branch);
}
