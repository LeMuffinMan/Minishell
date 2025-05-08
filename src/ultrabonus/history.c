/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:47:32 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/08 17:01:21 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "utils.h"
#include "history.h"
#include "env_utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <readline/readline.h> // compiler avec -l readline
#include <readline/history.h>

int add_node_to_history(char *cmd_line, t_hist **history)
{
  t_hist *node;
  t_hist *tmp;

  node = malloc(sizeof(t_hist)); 
  if (!node)
    return (-1);
  node->cmd_line = ft_strdup(cmd_line);
  if (!node->cmd_line)
  {
    //free node 
    return (-1);
  }
  if (!*history)
  {
    node->next = node;
    node->prev = node;
    *history = node;
  }
  else
  {
    tmp = *history;
    node->next = tmp;
    node->prev = tmp->prev;
    tmp->prev->next = node;
    tmp->prev = node;
  }
  return (0);
}

t_hist *get_history(char *file)
{
  int fd;
  char buffer[4096];
  ssize_t bytes_read;
  char *line_start;
  char *line_end;
  t_hist *history;

  history = malloc(sizeof(t_hist));
  if (!history)
    return (NULL);
  fd = open(file, O_RDONLY);
  if (fd == -1)
      return (NULL);
  bytes_read = read(fd, buffer, sizeof(buffer) - 1);
  close(fd);
  if (bytes_read <= 0)
      return (NULL);
  buffer[bytes_read] = '\0';
  line_start = buffer;
  line_end = ft_strchr(line_start, '\n');
  while (line_end)
  {
    *line_end = '\0';
    add_node_to_history(line_start, &history);
    line_start = line_end + 1;
    line_end = ft_strchr(line_start, '\n');
    if (!line_start)
      break;
  }
  return (history);
}

char *get_history_path(t_var **env)
{
  char *s;
  char *file;

  file = NULL;
  s = get_value(env, "HOME");
  if (s)
    file = s;
/*       HISTFILE : Chemin , du fichier sauvegardant l'historique (par défaut ~/.bash_history). */
  s = get_value(env, "HISTFILE");
  if (s)
    file = s;
  return (file);
}

int is_file_empty(const char *filename) {
    struct stat st;

    if (stat(filename, &st) != 0)
        return (-1);
    if (st.st_size == 0)
      return (0);
    else 
      return (1);
}

int is_valid_history_file(char *file)
{
  if (access(file, O_RDONLY | 644) != 0)
    return (0);
  if (is_file_empty(file))
    return (0);
  return (1);
}

int load_history(t_var **env, t_hist **history)
{
  char *file;
  char *tmp;

  file = NULL;
  file = get_history_path(env);
  if (!file)
  {
    file = get_value(env, "HOME");
    if (!file)
    {
      file = get_value(env, "USER");
      if (file)
      {
        file = ft_strjoin("/home/", file);
        tmp = file;
      }
    }
    if (file)
      file = ft_strjoin(file, "/.minishell_history");
    else
      file = ft_strjoin(file, ".minishell_history");
    if (tmp)
      free(tmp);
  } 
  if (is_valid_history_file(file)) //checker si fichier vide
    return (1);
  *history = get_history(file);
  if (!history)
  {
    //si il y a eu un probleme de malloc
    //// on free
    ///
    return (-1);
  }
  //on a pu remplir un historique
  return (0);
}

int free_history(t_hist **history)
{
  t_hist *tmp;

  tmp = *history;
  if (!*history)
    return (0);
  if (tmp->next == tmp)
  {
    free(tmp->cmd_line);
    free(tmp);
    return (0);
  }
  while (1)
  {
    free(tmp->prev->cmd_line);
    free(tmp->prev);
    if (!tmp->next)
    {
      free(tmp->cmd_line);
      free(tmp);
      return (0);
    }
    tmp = tmp->next;
  }
  history = NULL;
  return (0);
}
                 

int save_history(t_var **env, t_hist **history)
{
  int fd;
  t_hist *tmp;
  char *file;

  if (!history)
    return (1);
  file = get_history_path(env);
  if (!file)
    return (2);
  fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 644);
  if (fd == -1)
  {
    //open erro
  }
  tmp = *history; 
/*     HISTFILESIZE : Nombre maximal de commandes conservées dans le fichier d'historique. */
  //on check la variable histfilesize
  //si la tailel de history est plus grande
  //on avance en decrecmentant sa taille jusqu'a ce quelle soit egale a histfilesize
  while (1)
  {
    write(fd, tmp->cmd_line, ft_strlen(tmp->cmd_line));
    write(fd, "\n", 1);
    tmp = tmp->next;
    if (tmp == *history)
      break;
  }
  return (0);
  //normal et avec signaux !
  //a l'arret :
  //on copie en trunc sur le fichier d'historique toute la liste chainee de la fin au debut jusqu'a la limite HIST_SIZE ou une limite par defaut
}

int print_history(t_hist **history)
{
  t_hist *tmp;
  char *s;

  s = NULL;
  if (!*history)
    return (0);
  tmp = *history;
  if (tmp == *history)
  {
    s = ft_strjoin(" ", tmp->cmd_line); //printf padding ?
    ft_putstr_fd(s, 1);
    free(s);
    s = NULL;
    return (0);
  }
  while(1)
  {
    s = ft_strjoin(" ", tmp->cmd_line); //printf padding ?
    ft_putstr_fd(s, 1);
    free(s);
    s = NULL;
    if (tmp->next == *history)
      break ;
    tmp = tmp->next;
  }
  return (0);
}

int print_n_last_cmds(t_hist **history, int n)
{
  t_hist *tmp;
  char *s;

  s = NULL;
  if (n == 0 || !*history)
    return (0);
  tmp = (*history)->prev;
  while(1)
  {
    s = ft_strjoin(" ", tmp->cmd_line); //printf padding ?
    ft_putstr_fd(s, 1);
    free(s);
    s = NULL;
    n--;
    tmp = tmp->prev;
    if (n == 0 || tmp == *history)
      break ;
  }
  return (0);
}

int get_history_size(t_hist **history)
{
  t_hist *tmp;
  int i;

  if (!*history)
    return (0);
  if ((*history)->next == *history)
    return (1);
  i = 0;
  tmp = *history;
  while(1)
  {
    i++;
    if (tmp->next == *history)
      break ;
    tmp = tmp->next;
  }
  return (i);
}



int delete_n_history_node(t_hist **history, char *options)
{
  char **splitted;
  t_hist *tmp;
  int n;

  splitted = ft_split(options, ' ');
  if (!splitted)
    return (-1);
  if (splitted[0] && splitted[1] && get_history_size(history) <= ft_atoi(splitted[1]))
  {
    n = ft_atoi(splitted[1]);
    if (n == 0)
    {
      free_array(splitted);
      return (0); // 0 ?
    }
    tmp = *history;
    while (n > 0)
    {
      tmp = tmp->next;
      n--;
    }
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    free(tmp->cmd_line);
    free(tmp);
    return (0);
  }
  else
  {
    //syntax error ?
  }
  return (1); // on ne devrait pas arriver la 
}

int is_valid_option(char *options)
{
  (void)options;
  return (1); // a revoir
}


  //    history -c : Efface tout l'historique en mémoire (mais pas le fichier ~/.bash_history).
/*     history -d N : Supprime la commande à la ligne N de l'historique. */
/*     history -a : Écrit immédiatement les nouvelles commandes dans le fichier d'historique (~/.bash_history). */
/*     history -w : Écrase le fichier d'historique avec l'historique actuel en mémoire. */
/*     history -r : Recharge l'historique depuis le fichier ~/.bash_history. */
//si retourne 2 : on change le bouleen dans main
int execute_history_options(t_var **env, t_hist **history, char *options)
{
  if (is_valid_option(options))
  {
    //usage
    return (1);
  }
  if (ft_strchr(options, 'c'))
    return (free_history(history));
  else if (ft_strchr(options, 'd'))
    return (delete_n_history_node(history, options));
  else if (ft_strchr(options, 'a'))
    return (2);
  else if (ft_strchr(options, 'w'))
    return (save_history(env, history));
  else if (ft_strchr(options, 'r'))
  {
    if (history)
      free_history(history);
    return (load_history(env, history));
  }
  return (1); // on devrait pas arriver la
}

int is_numeric_only(char *s)
{
  int i;

  i = 0;
  while (s[i])
  {
    if (!ft_isdigit(s[i]))
      return (0);
    i++;
  }
  return (1);
}
  //history N : Affiche les N dernières commandes (ex: history 10).
int builtin_history(t_var **env, t_hist **history, char **arg)
{
  if (arg[0] && !arg[1])
    return (print_history(history));
  if (is_numeric_only(arg[1]))
    return (print_n_last_cmds(history, ft_atoi(arg[1])));
  return (execute_history_options(env, history, arg[1]));
}

int is_valid_string(char *line)
{
  int i;

  if (!line)
    return (0);
  if (line[0] != '!')
    return (0);
  if (line[1] != '?')
    return (0);
  i = 2;
  while (line[i] && line[i] != '?')
  {
    if (!ft_isalpha(line[i]) || ft_isdigit(line[i]))
      return (0);
    i++;
  }
  if (line[i] == '?' && !line[i + 1])
    return (1);
  return (0);
}

int is_only_alpha(char *line)
{
  int i;

  i = 0;
  while(line[i])
  {
    if (!ft_isalpha(line[i]))
      return (0);
    i++;
  }
  return (1);
}


int is_valid_history_cmd(char *line)
{
  if (line[0] != '!')
    return (0);
  //!!
  if (line[1] == '!' && !line[2])
    return (1);
  //!42
  else if (is_numeric_only(line + 1))
    return (1);
  //!git
  else if (is_only_alpha(line + 1))
    return (1);
  //!?git?
  else if (line[1] == '?')
  {
    if (line[ft_strlen(line) - 1] == '?')
    {
      line[ft_strlen(line) - 1] = '\0';
      if (is_only_alpha(line + 2))
      {
        line[ft_strlen(line) - 1] = '?';
        return (1);
      }
      else
      {
        line[ft_strlen(line) - 1] = '?';
        return (0);
      }
    }
  }
  else if (line[1] == '-')
  {
    //!-2
    if (is_numeric_only(line + 2))
      return (1);
    else 
      return (0);
  }
  return (0);
}

int execute_history_cmd(t_hist **history, char *line)
{
  char *cmd_line;
  t_hist *tmp;
  int n;
  char *s;

  if (is_valid_history_cmd(line))
  {
    //syntax error 
    return (1);
  }
/*     !! : Répète la dernière commande. */
  if (line[1] == '!')
  {
    cmd_line = (*history)->prev->cmd_line;
    //on remplace le prompt par la derniere commande
    return (0);
  }
/*     !N : Exécute la commande avec le numéro N dans l'historique (ex: !42). */
  if (is_numeric_only(line + 1))
  {
    n = ft_atoi(line + 1);
    tmp = *history; 
    while(n > 0)
    {
      tmp = tmp->next;
      n--;
    }
    cmd_line = tmp->cmd_line;
    //on remplace le prompt ou on execute direct ?
    return (0);
  }
/*     !-N : Exécute la commande qui était il y a N commandes (ex: !-2 pour l'avant-dernière). */
  if (line[1] == '-' && is_numeric_only(line + 2))
  {
    n = ft_atoi(line + 2);
    if (get_history_size(history) < n)
    {
      //erobash: !1: event not found
      return (128);
    }
    tmp = *history;
    while (n > 0)
    {
      tmp = (*history)->prev;
      n--;
    }
    cmd_line = tmp->cmd_line;
    //on execute direct ou on remplace le prompt
    return (0);
  }
  if (is_only_alpha(line + 1))
  {
    tmp = (*history)->prev;
    while (tmp->prev != *history)
    {
      if(ft_strnstr((line + 1), tmp->cmd_line, ft_strlen(line + 1)))
      {
        cmd_line = tmp->cmd_line;
      //on execute direct ou on remplace le prompt
        return (0);
      }
    }
    //bash: !ierugberuogerbniogergaaegrgaeg: event not found
    return (0);
  }
/*     !?string? : Exécute la dernière commande contenant string. */
  if (line[1] == '?') // pas bon
  {
    if (!is_valid_string(line))
    {
      //syntax error 
      return (1);
    }
    tmp = (*history)->prev;
    while (tmp->prev != *history)
    {
      if(ft_strnstr((line + 1), tmp->cmd_line, ft_strlen(line + 1) - 2)) // -2 ?
      {
        cmd_line = tmp->cmd_line;
      //on execute direct ou on remplace le prompt
        return (0);
      }
      s = ft_strnstr(tmp->cmd_line, (line + 1), ft_strlen(line + 1) - 1);
      if (ft_strncmp(s, tmp->cmd_line, ft_strlen(tmp->cmd_line)))
      {
        cmd_line = tmp->cmd_line;
        free(s);
      //on execute direct ou on remplace le prompt
        return (0);
      }
        free(s);
      s = NULL;
      tmp = tmp->prev;
    }
  //bash: !ierugberuogerbniogergaaegrgaeg: event not found
  }
  return (1); // on devrait pas arriver la
}

/*     HISTIGNORE : Permet de définir un motif pour exclure des commandes de l'historique (ex: export HISTIGNORE="ls:cd:exit"). */
int is_an_ignored_pattern(t_var **env, char *line)
{
  char *s;
  char **patterns_to_ignore;
  int i;

  s = get_value(env, "HISTIGNORE");
  if (!s)
    return (0);
  patterns_to_ignore = ft_split(s, ':');
  if (!patterns_to_ignore)
  {
    //malloc error 
  }
  i = 0;
  while(patterns_to_ignore[i])
  {
    if (ft_strnstr(line, patterns_to_ignore[i], ft_strlen(patterns_to_ignore[i])))
    {
      free_array(patterns_to_ignore);
      return (1);
    }
    i++;
  }
  free_array(patterns_to_ignore);
  return (0);
}



int is_valid_history_size(char *value)
{
  int i;

  i = 0;
  while(value[i])
  {
    if (!ft_isdigit(value[1]))
      return (0);
    i++;
  }
  return (1);
}

int ft_add_history(t_var **env, t_hist **history, char *line)
{
  char *s;
  int size;
  t_hist *tmp;
  int history_size;

  if (is_an_ignored_pattern(env, line))
    return (1);
  s = get_value(env, "HISTSIZE");
/*     HISTSIZE : Nombre maximal de commandes conservées en mémoire pour la session courante. */
  if (s)
  {
    if (!is_valid_history_size(s))
    {
      //la var est corrompue
      return (1);
    }
    size = ft_atoi(s); 
    history_size = get_history_size(history);
    if (history_size <= size)
    {
      if (add_node_to_history(line, history) == -1)
      {
        //malloc error 
      }
      add_history(line);
      return (0);
    }
    else 
    {
      tmp = *history;
      *history = tmp->next;
      tmp->prev->next = tmp->next;
      tmp->next->prev = tmp->prev;
      free(tmp->cmd_line);
      free(tmp);
      if (add_node_to_history(line, history) == -1)
      {
        //malloc error 
      }
      add_history(line);
      return (0);
    }
  }
  if (add_node_to_history(line, history) == -1)
  {
    //malloc error 
  }
  add_history(line);
  return (0);
}
/**/
/**/

