/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_gets_fcts_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:41:58 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:46:39 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "get_prompt.h"
#include "exec.h"

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

