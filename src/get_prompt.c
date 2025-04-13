#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

typedef struct s_prompt 
{
  char *ps1;  
  char *user;
  char *uid;
  char *hostname;
  char *pwd;
  char *git_branch;
  int total_len;
  char *prompt;
} t_prompt;


int get_value_len(t_var **env, t_prompt *prompt, char c)
{
  if (c == 'u')
    return (ft_strlen(prompt->user));
    /* return (ft_strlen(get_value(env, "USER"))); */
  if (c == 'h')
    return (ft_strlen(prompt->hostname));
    /* return (ft_strlen(get_value(env, "HOSTNAME"))); */
  if (c == 'W')
    return (ft_strlen("/home/muffin"));
    /* return (ft_strlen(get_value(env, "PWD"))); */
  if (c == 'g')
    return (ft_strlen(prompt->git_branch));
  else 
    return (0);
}

int get_prompt_len(char *s, int size, t_prompt *prompt)
{
  int i;
  int count;

  i = 0;
  count = 0;
  while (i < size)
  {
    if (s[i] == '\\' && i < (size - 1))
    {
      if (get_value_len(NULL, prompt, s[i+1]))
        count += get_value_len(NULL, prompt, s[i+1]);
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

char *get_uidline()
{
  int fd;
  char *line;

  fd = open("/proc/self/status", O_RDONLY);
  if (fd == -1)
      return (NULL);
  line = get_next_line(fd);
  while(line && ft_strncmp("Uid:", line, 4))
  {
    free(line);
    line = get_next_line(fd);
  }
  close(fd);
  return (line);
}

char *ft_getuid()
{
  char *line;
  char *uid;
  int fd;
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
  if (value)
  {
    *j += ft_strlcpy(prompt->prompt + *j, value, prompt->total_len + 4 - *j);
    *i += 2;
  }
  else
  {
    if (*j < prompt->total_len + 3) prompt->prompt[*j++] = prompt->ps1[*i++];
    if (*j < prompt->total_len + 3) prompt->prompt[*j++] = prompt->ps1[*i++];
  }
  return (0);
}

int copy_end_prompt(t_prompt *prompt, int i, int j, int len)
{
  if (i < len && j < prompt->total_len + 3)
      prompt->prompt[j++] = prompt->ps1[i];
  prompt->prompt[j++] = ' ';
  if (prompt->uid && !ft_strncmp(prompt->uid, "0", 2))
    prompt->prompt[j++] = '#';
  else
    prompt->prompt[j++] = '$';
  prompt->prompt[j] = '\0';
}

int expand_prompt(t_prompt *prompt, int len)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (prompt->ps1[i] && j < prompt->total_len + 3)
  {
    if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'u' && prompt->user)
      cpy_prompt_element(prompt, &i, &j, prompt->user);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'h' && prompt->hostname)
      cpy_prompt_element(prompt, &i, &j, prompt->hostname);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'W' && prompt->pwd)
      cpy_prompt_element(prompt, &i, &j, prompt->pwd);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1] && prompt->ps1[i + 1] == 'g' && prompt->git_branch)
      cpy_prompt_element(prompt, &i, &j, prompt->git_branch);
    else if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1])
      cpy_prompt_element(prompt, &i, &j, NULL);
    else if (j < prompt->total_len + 3)
      prompt->prompt[j++] = prompt->ps1[i++];
  }
  copy_end_prompt(prompt, i, j, len);
  return (0);
}

int get_prompt(t_prompt *prompt)
{
    int len;

    if (!prompt->ps1)
        return (1);
    len = ft_strlen(prompt->ps1);
    prompt->total_len = get_prompt_len(prompt->ps1, len, prompt);
    prompt->prompt = malloc(sizeof(char) * (prompt->total_len + 4)); // +4 pour $, espace et \0
    if (!prompt->prompt)
        return (1);
    expand_prompt(prompt, len);
    return (0);
}

char *trim_hostname(char *s)
{
  char *trimed;
  int i;
  int j;
  int len;

  i = 0;
  while (s[i] && !ft_isalnum(s[i]))
    i++;
  j = ft_strlen(s);
  while (j > 0 && !ft_isalnum(s[j]))
    j--;
  len = j - i;
  trimed = malloc(sizeof(char) * len + 1);
  j = 0;
  while (j < len)
  {
    trimed[j] = s[i];
    j++;
    i++;
  }
  trimed[j] = '\0';
  return (trimed);
}

char *get_hostname()
{
  char *line;
  char *hostname;
  int fd;

  fd = open("/etc/hostname", O_RDONLY);
  if (fd == -1) //retour d'erreur a refaire 
      return NULL;
  line = get_next_line(fd);
  close(fd);
  hostname = trim_hostname(line);
  free(line);
  return (hostname);
}

char *exctract_branch(char *path_to_head)
{
  int fd;
  ssize_t bytes_read;
  char *line;
  char *git_branch;
  char *tmp;

  line = NULL;
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
  else if (!ft_strncmp("Umask:	0ref: refs/heads/", line, ft_strlen("Umask:	0ref: refs/heads/")))
  {
    tmp = line;
    line += ft_strlen("ref: refs/heads/");
    git_branch = ft_strdup(line);
    free(tmp);
    return (git_branch);
  }
  //fixer ou changer de GNL !
  else if (!ft_strncmp("Gidref: refs/heads/", line, ft_strlen("Gidref: refs/heads/")))
  {
    tmp = line;
    line += ft_strlen("Gidref: refs/heads/") - 1;
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

char *trim_git_branch(char *s)
{
  int i;
  int start;
  int end;
  char *trimmed;

  if (!s)
    return (NULL);
  i = ft_strlen(s);
  while (i > 0 && s[i] != '/')
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
  path_to_head = ft_strjoin(pwd, "/.git/HEAD", NULL);
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
    path = ft_strjoin("/home/", dir->d_name, NULL);
    if (ft_strncmp(dir->d_name, ".", 2) == 0 || ft_strncmp(dir->d_name, "..", 3) == 0)
    {
      free(path);
      continue;
    }
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
  if (username)
  return (username);
}

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
  if (prompt->pwd)
    free(prompt->pwd);
  if (prompt->git_branch)
    free(prompt->git_branch);
  if (prompt->prompt)
    free(prompt->prompt);
  free(prompt);
  return (0);
}

int main (void)
{
  t_prompt *prompt;
  char *ps1_fake = "[\\u@\\h Minishell \\W] \\g";
  char *pwd_fake = "/home/muffin";

  prompt = malloc(sizeof(t_prompt));
  prompt->ps1 = ft_strdup(ps1_fake);
/*prompt->ps1 = get_value(env, "PS1");*/
  prompt->user = NULL;
  /* prompt->user = get_value(env, "USER"); */
  prompt->uid = ft_getuid();
  if (!ft_strncmp(prompt->uid, "0", 2))
    prompt->user = "root";
  if (!prompt->user)
    prompt->user = get_user(); //on fait un get_value, si il foire on fait un get_user
  prompt->hostname = get_hostname();
  prompt->pwd = ft_strdup(pwd_fake);
  /* prompt->pwd = get_value(env, "PWD"); */
  prompt->git_branch = get_branch("/home/muffin/42/Minishell");
  prompt->total_len = 0;
  prompt->prompt = NULL;
  get_prompt(prompt);
  if (prompt->prompt)
    printf("%s\n", prompt->prompt);
  free_prompt(prompt);
  return (0);
}
