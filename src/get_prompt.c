#include "minishell.h"
#include "libft.h"

typedef struct s_prompt 
{
  char *ps1;  
  char *user;
  char *hostname;
  char *pwd;
  int total_len;
} t_prompt;

int get_value_len(t_var **env, char c)
{
  if (c == 'u')
    return (ft_strlen(get_value(env, "USER")));
  if (c == 'h')
    return (ft_strlen(get_value(env, "HOSTNAME")));
  if (c == 'W')
    return (ft_strlen(get_value(env, "PWD")));
  else 
    return (0);
}

int get_prompt_len(char *s, int size)
{
  int i;
  int value_len;
  int count;

  i = 0;
  count = 0;
  while (i < size)
  {
    if (ps1[i] == '\\' && i < (len - 1))
    {
      value_len = get_value_len(env); 
      if (value_len)
        count+=value_len;
      else 
      {
        count+=2;
        i+=2;
      }
    }
    else
    {
      count++;
      i++;
    }
  }
}

int prompt_expand(char *prompt, char *value, int start)
{
  int i;

  if (!value)
    return (NULL); //est-ce que je vais chercher le user avec la strat access ? 
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
  return (i);
}

char *get_ps1(t_var **env);
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
  res = '$'
  while (line)
  {
    if (!ft_strncmp(line, "Uid:", 4))
    {
      uid = extract_uid(line));
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

char *get_prompt(t_var **env)
{
  t_prompt prompt;
  int i;
  int len;

  prompt->ps1 = get_ps1(env);
  if (!promt->ps1)
    return (NULL);
  len = ft_strlen(prompt->ps1);
  i = 0;
  prompt.total_len = get_prompt_len(prompt->ps1, len);
  prompt->prompt = malloc(sizeof(char) * (prompt.total_len) + 2) //pour le $ et le \0
  while(i < prompt.total_len + 2)
  {
    if (ps1[i] == '\\' && i < prompt.total_len)
    {
      switch (ps1[i + 1])
        case 'u':
          i+=prompt_expand(prompt->prompt, prompt->user, i);
          break;
        case 'h':
          i+=prompt_expand(prompt->prompt, prompt->hostname, i);
          break;
        case 'W':
          i+=prompt_expand(prompt->prompt, prompt->pwd, i);
          break;
        default:
          if (i < prompt.total_len) 
            i+=2;
          else
            i++;
        break;
    }
    else
    {
      prompt[i] = ps1[i];
      i++;
    }
  }
  prompt->prompt[i] = ft_getuid();
  i++;
  prompt->prompt[i] = '\0';
  return (prompt->prompt);
}
