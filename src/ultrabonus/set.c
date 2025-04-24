/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:28:12 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:28:14 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */



int print_aliases(t_var **env)
{
  t_var *tmp;
  char *s;
  char *temp;

  tmp = *env;
  while(tmp)
  {
    if (tmp->alias == 1))
    {
      s = ft_strjoin(tmp->key, "=");
      temp = s;
      s = ft_strjoin(s, tmp->value);
      free(temp);
      s = ft_strjoin(s, "\n");
      free(temp);
      ft_putstr_fd(s, 1);
    }
    tmp = tmp->next;
  }
  return(0);
}


int print_shell_loc(t_var **env)
{
  t_var *tmp;
  char *s;
  char *temp;

  tmp = *env;
  while(tmp)
  {
    if (!ft_strncmp(tmp->key, "MINISHELL", 10))
    {
      s = ft_strjoin(tmp->key, "=");
      temp = s;
      s = ft_strjoin(s, tmp->value);
      free(temp);
      s = ft_strjoin(s, ,"\n");
      free(temp);
      ft_putstr_fd(s, 1);
    }
    tmp = tmp->next;
  }
  return(0);
}

int print_shell_functions(t_var **env)
{
  t_var *tmp;
  int i;

  tmp = *env;
  while(tmp)
  {
    if (!ft_strncmp(tmp->shell_fct == 1))
    {
      ft_putstr_fd(tmp->key, 1);
      ft_putstr_fd("\n{\n", 1);
      i = 0;
      while(tmp->content[i])
      {
        ft_putstr_fd(tmp->content[i]);
        i++;
      }
      ft_putstr_fd("\n}\n", 1);
    }
    tmp = tmp->next;
  }
  return (0);
}

int print_default_variables(t_var **env)
{
  ft_putstr_fd("MINISHELL=/usr/bin/minishell", 1); //a revoir !!
  ft_putstr_fd("ALIASES=()", 1);
  ft_putstr_fd("ARGC=([0]=\"0\"", 1);
  ft_putstr_fd("ARGV=()", 1);
  ft_putstr_fd("BASH_CMDS=()", 1); //faire un print startup cmd ?
  ft_putstr_fd("BASH_SOURCE=()", 1);
  return (0);
}

int print_env_variables(t_var **env)
{
  t_var *tmp;

  tmp = *env;
  while(tmp)
  {
    if (!ft_strncmp(tmp->env == 1))
    {
      //on concatene avant ?
      ft_putstr_fd(tmp->key, 1);
      ft_putstr_fd("=", 1);
      ft_putstr_fd(tmp->value, 1);
      ft_putstr_fd("\n", 1);
    }
    tmp = tmp->next;
  }
}

int print_exported_variables(t_var **env)
{
  t_var *tmp;

  tmp = *env;
  while(tmp)
  {
    if (!ft_strncmp(tmp->export == 1))
    {
      //on concatene avant ?
      ft_putstr_fd(tmp->key, 1);
      ft_putstr_fd("=", 1);
      ft_putstr_fd(tmp->value, 1);
      ft_putstr_fd("\n", 1);
    }
    tmp = tmp->next;
  }
  return (0);
}

int builtin_set(t_var **env, char **content);
{
  if (content[1] && !ft_strncmp(content[1], "--aliases", 10))
    return(print_aliases(env));
  if (content[1] && !ft_strncmp(content[1], "--shell-fcts", 13))
    return (print_shell_functions(env));
  else
  {
    print_default_variables(env);
    print_env_variables(env);
    print_exported_variables(env);
    //imprimer _ separement toujours en dernier ?
    return (0);
  }
  return(0);
}
