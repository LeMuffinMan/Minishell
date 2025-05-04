/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:27:37 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:27:38 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* #include "minishell.h" */
#include "get_prompt.h"
#include "exec.h"
#include "env_utils.h"
#include "get_next_line.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


/* int prompt_expand(char *prompt, char *value, int start) */
/* { */
/*   int i; */
/**/
/*   if (!value) */
/*     return (2); */
    /* return (NULL); //est-ce que je vais chercher le user avec la strat access ?  */
/*   //si il a access sur all, c'est root OU admin */
/*   //si getuid() == 0 USER = root */
/*   //si il a acces a plus de 1 dossier c'est un admin USER = "" */
/*   i = 0; */
/*   while(value[i]) */
/*   { */
/*     prompt[start] = value[i]; */
/*     start++; */
/*     i++; */
/*   } */
/*   if (i < 2) */
/*     i = 2; */
/*   return (i); */
/* } */

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

char *trim_ps1(char *ps1)
{
  char *trimmed;
  long unsigned int i;

  i = 0;
  trimmed = malloc(sizeof(char) * ft_strlen(ps1) - 2);
  if (!trimmed)
    return (NULL);
  while(ps1[i] && ps1[i + 1] != '\'' && ps1[i + 1] != '\"')
  {
    trimmed[i] = ps1[i + 1];
    i++;
  }
  trimmed[i] = '\0';
  free(ps1);
  return (trimmed);
}

char *clean_quotes(char *ps1)
{
  int i;
  char *trimmed;

  i = 0;
  trimmed = NULL;
  if (ps1[i] == '\'' && ps1[ft_strlen(ps1) - 1] == '\'')
    trimmed = trim_ps1(ps1);
  if (!trimmed)
    return (NULL);
  if (ps1[i] == '\"' && ps1[ft_strlen(ps1) - 1] == '\"')
    trimmed = trim_ps1(ps1);
  if (!trimmed)
    return (NULL);
  return (trimmed);
}

char *read_ps1(t_var **env)
{
  char *ps1;
  char *new_ps1;

  ps1 = ft_strdup(get_value(env, "PS1"));
  if ((ps1 && ps1[0] == '\'' && ps1[ft_strlen(ps1) - 1] == '\'') || 
    (ps1 && ps1[0] == '\"' && ps1[ft_strlen(ps1) - 1] == '\"')) 
  {
    new_ps1 = ft_substr(ps1, 1, ft_strlen(ps1) - 2);
    free(ps1);
    if (!new_ps1)
      return (NULL);
    return (new_ps1);
  }
  return (ps1);
}

char *get_prompt (t_var **env)
{
  t_prompt *prompt;
  char *ps1_fake = "[\\u \\W \\g] \\$";
  char *expanded_prompt;

  prompt = malloc(sizeof(t_prompt));
  prompt->ps1 = read_ps1(env);
  if (!prompt->ps1)
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
