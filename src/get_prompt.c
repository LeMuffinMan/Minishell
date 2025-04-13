#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

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


int get_value_len(t_var **env, char c)
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
  else 
    return (0);
}

///* int get_prompt_len(char *s, int size/* , t_env **env */) */
/* { */
/*   int i; */
/*   int value_len; */
/*   int count; */
/**/
/*   i = 0; */
/*   count = 0; */
/*   while (i < size) */
/*   { */
/*     if (s[i] == '\\' && i < (size - 1)) */
/*     { */
      /* value_len = get_value_len(env);  */
/*       value_len = ft_strlen("muffin"); */
/*       if (value_len) */
/*         count+=value_len; */
/*       else  */
/*         count+=2; */
/*         i+=2; */
/*     } */
/*     else */
/*     { */
/*       count++; */
/*       i++; */
/*     } */
/*   } */
/* } */

int get_prompt_len(char *s, int size)
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
      value_len = get_value_len(NULL, s[i+1]);
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

/* char *get_prompt(char *ps1, t_prompt *prompt) */
/* { */
/*   int i; */
/*   int len; */
/**/
/*   prompt->ps1 = ps1; */
/*   if (!prompt->ps1) */
/*     return (NULL); */
/*   len = ft_strlen(prompt->ps1); */
/*   i = 0; */
  //prompt->total_len = get_prompt_len(prompt->ps2, len/* , env */);
/*   prompt->prompt = malloc(sizeof(char) * prompt->total_len + 4); //pour le $ et le \0 */
  /* printf("allocated : %d\n", prompt->total_len + 4); */
/*   while(i < prompt->total_len + 4) */
/*   { */
/*     printf("i = %d\nlen + 4 = %d\n", i, prompt->total_len + 4); */
/*     if (prompt->ps1[i] == '\\' && i < prompt->total_len + 4) */
/*     { */
/*       if (prompt->user && prompt->ps1[i + 1] == 'u') */
/*         i+=prompt_expand(prompt->prompt, prompt->user, i); */
/*       else if (prompt->hostname && prompt->ps1[i + 1] == 'h') */
/*         i+=prompt_expand(prompt->prompt, prompt->hostname, i); */
/*       else if (prompt->pwd && prompt->ps1[i + 1] == 'W') */
/*         i+=prompt_expand(prompt->prompt, prompt->pwd, i); */
/*       else  */
/*         i+=2; */
/*     } */
/*     else */
/*     { */
/*       prompt->prompt[i] = prompt->ps1[i]; */
/*       i++; */
/*     } */
/*   } */
/*   prompt->prompt[i] = '$'; */
/**/
/*   i++; */
/*   prompt->prompt[i] = '\0'; */
/*   return (prompt->prompt); */
/* } */

/* char *get_prompt(char *ps1, t_prompt *prompt) */
/* { */
/*     int i; */
/*     int j; */
/*     int len; */
/**/
/*     prompt->ps1 = ps1; */
/*     if (!prompt->ps1) */
/*         return (NULL); */
/*     len = ft_strlen(prompt->ps1); */
/*     prompt->total_len = get_prompt_len(prompt->ps1, len); */
/*     prompt->prompt = malloc(sizeof(char) * (prompt->total_len + 4)); // +4 pour $, espace et \0 */
/*     if (!prompt->prompt) */
/*         return (NULL); */
/*     i = 0; */
/*     j = 0; */
/*     while (prompt->ps1[i] && j < prompt->total_len + 3) // +3 pour $, espace et \0 */
/*     { */
/*         if (prompt->ps1[i] == '\\' && prompt->ps1[i + 1]) */
/*         { */
/*             if (prompt->user && prompt->ps1[i + 1] == 'u') */
/*             { */
/*                 j += ft_strlcpy_prompt(prompt->prompt + j, prompt->user, prompt->total_len + 4 - j); */
/*                 i += 2; */
/*             } */
/*             else if (prompt->hostname && prompt->ps1[i + 1] == 'h') */
/*             { */
/*                 j += ft_strlcpy_prompt(prompt->prompt + j, prompt->hostname, prompt->total_len + 4 - j); */
/*                 i += 2; */
/*             } */
/*             else if (prompt->pwd && prompt->ps1[i + 1] == 'W') */
/*             { */
/*                 j += ft_strlcpy_prompt(prompt->prompt + j, prompt->pwd, prompt->total_len + 4 - j); */
/*                 i += 2; */
/*             } */
/*             else  */
/*             { */
/*                 prompt->prompt[j++] = prompt->ps1[i++]; */
/*                 prompt->prompt[j++] = prompt->ps1[i++]; */
/*             } */
/*         } */
/*         else */
/*         { */
/*             prompt->prompt[j++] = prompt->ps1[i++]; */
/*         } */
/*     } */
/*     prompt->prompt[j++] = ' '; */
/*     prompt->prompt[j++] = '$'; */
  /* prompt->prompt[i] = ft_getuid(); */
/*     prompt->prompt[j] = '\0'; */
/*     return (prompt->prompt); */
/* } */

char *get_prompt(char *ps1, t_prompt *prompt)
{
    int i;
    int j;
    int len;

    prompt->ps1 = ps1;
    if (!prompt->ps1)
        return (NULL);
    len = ft_strlen(prompt->ps1);
    prompt->total_len = get_prompt_len(prompt->ps1, len);
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


int main (void)
{
  char *expanded_prompt;
  /* prompt->ps1 = get_ps1(env); */
  /* prompt = get_prompt(prompt->ps1); */
  t_prompt *prompt;

  prompt = malloc(sizeof(t_prompt));
  prompt->ps1 = NULL;  
  prompt->user = "muffin";
  prompt->uid = NULL;
  prompt->hostname = "Arch";
  prompt->pwd = "/home/muffin";
  prompt->total_len = 0;
  prompt->prompt = NULL;
  expanded_prompt = get_prompt("[\\u@\\h Minishell \\W]", prompt);
  printf("%s\n", expanded_prompt);
  free(expanded_prompt);
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
