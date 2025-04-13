
#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

char *exctract_branch(char *path_to_head)
{
  int fd;
  ssize_t bytes_read;
  char *line;
  char *git_branch;
  char *tmp;

  fd = open(path_to_head, O_RDONLY);
  if (fd == -1) //retour d'erreur a refaire 
      return NULL;
  line = get_next_line(fd);
  close(fd);
  if (!ft_strncmp("ref: refs/heads/", line, ft_strlen("ref: refs/heads/")))
  {
    tmp = line;
    line += ft_strlen("ref: refs/heads/");
    git_branch = ft_strdup(line);
    free(tmp);
    return (git_branch);
  }
  else 
  {
    free(line);
    return (NULL);
  }
}

char *get_branch (char *pwd)
{
  char *git_branch;
  char *path_to_head;
  char *tmp;

  path_to_head = ft_strjoin(pwd, "/.git/HEAD", NULL);
  if (access(path_to_head, F_OK | R_OK) == 0)
  {
    git_branch = exctract_branch(path_to_head);
    free(path_to_head);
    tmp = git_branch;
    git_branch = ft_strtrim(git_branch, "\n");
    free(tmp);
    return (git_branch);
  }
  free(path_to_head);
  return (git_branch);
}

int main (void)
{
  char *git_branch;
  
  git_branch = get_branch("/home/muffin/42/Minishell");
  if (git_branch)
  {
    printf("%s\n", git_branch);
    free(git_branch);
  }
  return (0);
}
