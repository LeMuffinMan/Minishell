#include "minishell.h"
#include "get_next_line.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int free_prompt(t_prompt *prompt)
{
  if (!prompt)
    return (1);
  if (prompt->ps1)
    free(prompt->ps1);  
  if (prompt->user)
    free(prompt->user);
  if (prompt->uid)
    free(prompt->uid);
  if (prompt->hostname)
    free(prompt->hostname);
  if (prompt->hostname_long)
    free(prompt->hostname_long);
  if (prompt->pwd)
    free(prompt->pwd);
  if (prompt->git_branch)
    free(prompt->git_branch);
  if (prompt->user_type)
    free(prompt->user_type);
  if (prompt->prompt)
    free(prompt->prompt);
  free(prompt);
  return (0);
}

int get_value_len(t_var **env, t_prompt *prompt, char c)
{
  (void)env;
  if (c == 'u' && prompt->user)
    return (ft_strlen(prompt->user));
  if (c == 'h' && prompt->hostname)
    return (ft_strlen(prompt->hostname));
  if (c == 'H' && prompt->hostname_long)
    return (ft_strlen(prompt->hostname_long));
  if (c == 'W' && prompt->pwd)
    return (ft_strlen(prompt->pwd));
  if (c == 'g' && prompt->git_branch)
    return (ft_strlen(prompt->git_branch));
  if (c == '$' && prompt->uid)
    return (1);
  else 
    return (0);
}

int get_prompt_len(char *s, int size, t_prompt *prompt, t_var **env)
{
  int i;
  int count;

  i = 0;
  count = 0;
  while (i < size)
  {
    if (s[i] == '\\' && i < (size - 1))
    {
      if (get_value_len(env, prompt, s[i+1]))
        count += get_value_len(env, prompt, s[i+1]);
      else 
        count += 2;
      i += 2;
    }
    else
    {
      count++;
      i++;
    }
  }
  return (count);
}


int prompt_expand(char *prompt, char *value, int start)
{
  int i;

  if (!value)
    return (2);
    /* return (NULL); //est-ce que je vais chercher le user avec la strat access ?  */
  //si il a access sur all, c'est root OU admin
  //si getuid() == 0 USER = root
  //si il a acces a plus de 1 dossier c'est un admin USER = ""
  i = 0;
  while(value[i])
  {
    prompt[start] = value[i];
    start++;
    i++;
  }
  if (i < 2)
    i = 2;
  return (i);
}

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

int extract_uid(char *line)
{
  int i;
  int j;
  char *uid;
  int res;

  i = 0;
  j = 0;
  while(!ft_isdigit(line[i]))
    i++;
  while(ft_isdigit(line[i + j]))
    j++;
  uid = malloc(sizeof(char) * (j - i) + 1);
  j = 0;
  while (ft_isdigit(line[i]))
  {
    uid[j] = line[i];
    i++;
    j++;
  }
  uid[j] = '\0';
  res = ft_atoi(uid); 
  free(uid);
  return (res);
}

char *trim_hostname(char *s, int mode)
{
  char *trimed;
  int i;
  int j;
  int len;

  //faire un confition pour trim au 1er . si petit h 
  if (!s)
    return (NULL);
  i = 0;
  while (s[i] && !ft_isalnum(s[i]))
    i++;
  if (mode)
  {
    j = ft_strlen(s) - 1;
    while (j > 0 && s[j] && !ft_isalnum(s[j]))
      j--;
    len = j - i;
  }
  else
  {
    j = 0; //ici on change pour /H
    while (s[j] && s[j] != '.')
      j++;
    len = j - 1 - i;
  }
  trimed = malloc(sizeof(char) * len + 2);
  j = 0;
  while (j <= len)
  {
    trimed[j] = s[i];
    j++;
    i++;
  }
  trimed[j] = '\0';
  return (trimed);
}

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

char *tilde_replace(char *s, t_var **env)
{
  int i;
  int j;
  char *new_path;

  if (!ft_strncmp(s, get_value(env, "HOME"), ft_strlen(get_value(env, "HOME"))))
  {
    new_path = malloc(sizeof(char) * ft_strlen(s) - ft_strlen(get_value(env, "HOME")) + 2);
    new_path[0] = '~';
    i = ft_strlen(get_value(env, "HOME"));
    j = 1;  
    while (s[i])
    {
      new_path[j] = s[i];
      i++;
      j++;
    }
    new_path[j] = '\0';
    free(s);
    return (new_path);
  }
  return (s);
}

char *get_branch(char *pwd)
{
  char *git_branch;
  char *path_to_head;
  char *tmp;

  git_branch = NULL;
  path_to_head = ft_strjoin(pwd, "/.git/HEAD");
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

int cpy_prompt_element(t_prompt *prompt, int *i, int *j, char *value)
{
  int len;

  /* printf("[%s]\n", value); */
  if (value)
  {
    len = ft_strlen(value);
    if (*j + len < prompt->total_len + 4)
    {
      ft_strlcpy(prompt->prompt + *j, value, len + 1);
      *j += len;
      *i += 2;
    }
  }
  else
    *i+=2;
  return (0);
}

int copy_end_prompt(t_prompt *prompt, int i, int j, int len)
{
  if (i < len && j < prompt->total_len + 3)
      prompt->prompt[j++] = prompt->ps1[i];
  /* if (prompt->user_type) */
  /*   prompt->prompt[j++] = prompt->user_type[0]; */
  prompt->prompt[j++] = ' ';
  prompt->prompt[j] = '\0';
  return (0);
}

int expand_prompt(t_prompt *prompt, int len)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (prompt->ps1[i] && j < prompt->total_len + 3)
  {
    if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'u')
      cpy_prompt_element(prompt, &i, &j, prompt->user);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'h')
      cpy_prompt_element(prompt, &i, &j, prompt->hostname);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'H')
      cpy_prompt_element(prompt, &i, &j, prompt->hostname_long);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'W')
      cpy_prompt_element(prompt, &i, &j, prompt->pwd);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'g')
      cpy_prompt_element(prompt, &i, &j, prompt->git_branch);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == '$')
      cpy_prompt_element(prompt, &i, &j, prompt->user_type);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'n')
      cpy_prompt_element(prompt, &i, &j, "\n");
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1])
      cpy_prompt_element(prompt, &i, &j, NULL);
    else if (j < prompt->total_len + 3)
      prompt->prompt[j++] = prompt->ps1[i++];
  }
  copy_end_prompt(prompt, i, j, len);
  return (0);
}

int build_prompt(t_prompt *prompt, t_var **env)
{
    int len;

    if (!prompt->ps1)
        return (1);
    len = ft_strlen(prompt->ps1);
    prompt->total_len = get_prompt_len(prompt->ps1, len, prompt, env);
    prompt->prompt = malloc(sizeof(char) * (prompt->total_len + 4 + 1)); 
    if (!prompt->prompt)
        return (1);
    ft_memset(prompt->prompt, 0, prompt->total_len + 4 + 1);
    expand_prompt(prompt, len);
    return (0);
}

char *get_prompt (t_var **env)
{
  t_prompt *prompt;
  char *ps1_fake = "[\\u@\\h \\H \\W \\g] \\$";
  char *expanded_prompt;

  prompt = malloc(sizeof(t_prompt));
  if (get_value(env, "PS1"))
    prompt->ps1 = ft_strdup(get_value(env, "PS1"));
  else
    prompt->ps1 = ft_strdup(ps1_fake);
  /* printf("%s\n", get_value(env, "PS1")); */
/*prompt->ps1 = get_value(env, "PS1");*/
  prompt->uid = ft_getuid();
  if (prompt->uid == 0)
    prompt->user_type = ft_strdup("#");
  else
    prompt->user_type = ft_strdup("$");
  if (!ft_strncmp(prompt->uid, "0", 2))
    prompt->user = ft_strdup("root");
  prompt->user = ft_strdup(get_value(env, "USER"));
  if (!prompt->user)
    prompt->user = get_user(); //on fait un get_value, si il foire on fait un get_user
  prompt->hostname = get_hostname(0);
  prompt->hostname_long = get_hostname(1);
  prompt->pwd = ft_strdup(get_value(env, "PWD"));
  prompt->git_branch = get_branch(prompt->pwd);
  prompt->pwd = tilde_replace(prompt->pwd, env);
  prompt->total_len = 0;
  prompt->prompt = NULL;
  build_prompt(prompt, env);
  expanded_prompt = ft_strdup(prompt->prompt);
  free_prompt(prompt);
  return (expanded_prompt);
}

//interpreter comme bash : 
// double backslash pour que ce soit recu comme ca
// permettre les quote pour mettre des espaces 
//ajouter le \w 
////\l : pour afficher bash / minishell
//\s :  the name of the shell, the basename of $0 (the portion following the final slash)
