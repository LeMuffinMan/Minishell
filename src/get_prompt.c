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


int get_value_len(t_var **env, char *git_branch, char c)
{
  if (c == 'u')
    /* return (ft_strlen(get_value(env, "USER"))); */
    return (ft_strlen("muffin"));
  if (c == 'h')
    /* return (ft_strlen(get_value(env, "HOSTNAME"))); */
    return (ft_strlen("Arch"));
  if (c == 'W')
    /* return (ft_strlen(get_value(env, "PWD"))); */
    return (ft_strlen("/home/muffin"));
  if (c == 'g')
    return (ft_strlen(git_branch));
  else 
    return (0);
}

int get_prompt_len(char *s, int size, t_prompt *prompt)
{
  int i;
  int value_len;
  int count;

  i = 0;
  count = 0;
  while (i < size)
  {
    if (s[i] == '\\' && i < (size - 1))
    {
      value_len = get_value_len(NULL, prompt->git_branch, s[i+1]);
      if (value_len)
        count += value_len;
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

char ft_getuid()
{
  char *line;
  int uid;
  char res;
  //open /proc/self/status
  //gnl dessus 
  uid = -1;
  res = '$';
  while (line)
  {
    if (!ft_strncmp(line, "Uid:", 4))
    {
      uid = extract_uid(line);
      free(line);
      break;
    }
    free(line);
    //gnl
  }
  if (uid == 0)
    res = '#';
  return (res);
}

char *get_prompt(char *ps1, t_prompt *prompt)
{
    int i;
    int j;
    int len;

    prompt->ps1 = ps1;
    if (!prompt->ps1)
        return (NULL);
    len = ft_strlen(prompt->ps1);
    prompt->total_len = get_prompt_len(prompt->ps1, len, prompt);
    prompt->prompt = malloc(sizeof(char) * (prompt->total_len + 4)); // +4 pour $, espace et \0
    if (!prompt->prompt)
        return (NULL);
    i = 0;
    j = 0;
    while (prompt->ps1[i] && j < prompt->total_len + 3) // +3 pour $, espace et \0
    {
        if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1])
        {
            if (prompt->user && prompt->ps1[i + 1] == 'u')
            {
                j += ft_strlcpy(prompt->prompt + j, prompt->user, prompt->total_len + 4 - j);
                i += 2;
            }
            else if (prompt->hostname && prompt->ps1[i + 1] == 'h')
            {
                j += ft_strlcpy(prompt->prompt + j, prompt->hostname, prompt->total_len + 4 - j);
                i += 2;
            }
            else if (prompt->pwd && prompt->ps1[i + 1] == 'W')
            {
                j += ft_strlcpy(prompt->prompt + j, prompt->pwd, prompt->total_len + 4 - j);
                i += 2;
            }
            else if (prompt->git_branch && prompt->ps1[i + 1] == 'g')
            {
                j += ft_strlcpy(prompt->prompt + j, prompt->git_branch, prompt->total_len + 4 - j);
                i += 2;
            }
            else 
            {
                if (j < prompt->total_len + 3) prompt->prompt[j++] = prompt->ps1[i++];
                if (j < prompt->total_len + 3) prompt->prompt[j++] = prompt->ps1[i++];
            }
        }
        else
        {
            if (j < prompt->total_len + 3) prompt->prompt[j++] = prompt->ps1[i++];
        }
    }
    if (i < len && j < prompt->total_len + 3)
        prompt->prompt[j++] = prompt->ps1[i];
    
    prompt->prompt[j++] = ' ';
  //get_uid()
    prompt->prompt[j++] = '$';
    prompt->prompt[j] = '\0';
    return (prompt->prompt);
}


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

/* int main (void) */
/* { */
/*   char *git_branch; */
/*    */
/*   git_branch = get_branch("/home/muffin/42/Minishell"); */
/*   if (git_branch) */
/*   { */
/*     printf("%s\n", git_branch); */
/*     free(git_branch); */
/*   } */
/*   return (0); */
/* } */

char *get_user()
{
  DIR *d;
  struct dirent *dir;
  char *username;
  char *path;

  //on cherche d'abord l'uid pour assigner user root ?
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

int main (void)
{
  char *expanded_prompt;
  /* prompt->ps1 = get_ps1(env); */
  /* prompt = get_prompt(prompt->ps1); */
  t_prompt *prompt;

  prompt = malloc(sizeof(t_prompt));
  prompt->ps1 = NULL;  
/*prompt->ps1 = get_value(env, "PS1");*/
  prompt->user = NULL;
  /* prompt->user = get_value(env, "USER"); */
  if (!prompt->user)
    prompt->user = get_user(); //on fait un get_value, si il foire on fait un get_user
  prompt->uid = NULL;
  /* prompt->uid = get_uid(); */
  prompt->hostname = "Arch";
  /* prompt->hostname = get_hostname(); */
  prompt->pwd = "/home/muffin";
  /* prompt->pwd = get_value(env, "PWD"); */
  prompt->git_branch = get_branch("/home/muffin/42/Minishell");
  prompt->total_len = 0;
  prompt->prompt = NULL;
  expanded_prompt = get_prompt("[\\u@\\h Minishell \\W] \\g", prompt);
  if (expanded_prompt)
    printf("%s\n", expanded_prompt);
  free(expanded_prompt);
  free(prompt->git_branch);
  free(prompt->user);
  free(prompt);
  return (0);
}

//Git 
//
//get git branch
//  is a git dir ?
//  build .git/HEAD
//  open 
//  read
//  chercher : refs: 
//  extraire le bout du path qui suit
//  close
//
