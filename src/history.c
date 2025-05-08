/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:47:32 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 21:00:16 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

typedef strcut s_hist
{
  char *cmd_line;
  struct s_hist *next;
  struct s_hist *prev;
} t_hist;

int add_node_to_history(char *cmd_line, t_hist **history, int index)
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

t_hist **get_history(char *file)
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
  index = get_index(buffer);
  while (line_end)
  {
    *line_end = '\0';
    add_node_to_history(line_start, &history, index);
    index--;
    line_start = line_end + 1;
    line_end = ft_strchr(line_start, '\n');
    if (!line_start)
      break;
  }
  return (history);
}

char *get_history_path(t_var **env)
{
  t_var *node;
  char *file;

  file = NULL;
  node = get_value(env, "HOME");
  if (node)
    file = node->value;
  node = get_value(env, "HISTFILE");
  if (node)
    file = node->value;
  return (file);
}

int read_history(t_var **env, t_hist *history)
{
  char *file;

  file = NULL;
  file = get_history_path(env);
  if (!file)
    return (1);
  if (is_valid_history_file(file)) //checker si fichier vide
    return (1);
  history = get_history(file);
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

  if (!history)
    return (1);
  file = get_history_path(env);
  if (!file)
    return (2);
  fd = open(file, O_WRONLY | O_TRUNC, 644);
  if (fd == -1)
  {
    //open erro
  }
  tmp = *history; 
  //on check la variable histfilesize
  //si la tailel de history est plus grande
  //on avance en decrecmentant sa taille jusqu'a ce quelle soit egale a histfilesize
  while (1);
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
    s = ft_strjoin(' ', tmp->cmd_line); //printf padding ?
    ft_putstr_fd(s, 1);
    free(s);
    s = NULL;
    return (0);
  }
  while(1)
  {
    s = ft_strjoin(' ', tmp->cmd_line); //printf padding ?
    ft_putstr_fd(s, 1);
    free(s);
    s = NULL;
    if (tmp->next == *history)
      break ;
    tmp = tmp->next;
  }
  return (0);
}

int print_n_last_cmds(t_var **history, int n)
{
  t_hist *tmp;
  char *s;

  s = NULL;
  if (n == 0 || !*history)
    return (0);
  tmp = (*history)->prev;
  while(1)
  {
    s = ft_strjoin(' ', tmp->cmd_line); //printf padding ?
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



  //on open le fichier history et on affiche chaque ligne avec un numerottage au debut 
  //history N : Affiche les N dernières commandes (ex: history 10).

/**/
/**/
/*     !N : Exécute la commande avec le numéro N dans l'historique (ex: !42). */
/**/
/*     !! : Répète la dernière commande. */
/**/
/*     !-N : Exécute la commande qui était il y a N commandes (ex: !-2 pour l'avant-dernière). */
/**/
/*     !string : Exécute la dernière commande commençant par string (ex: !git). */
/**/
/*     !?string? : Exécute la dernière commande contenant string. */
/*       HISTFILE : Chemin , du fichier sauvegardant l'historique (par défaut ~/.bash_history). */
/**/
/*     HISTSIZE : Nombre maximal de commandes conservées en mémoire pour la session courante. */
/**/
/*     HISTFILESIZE : Nombre maximal de commandes conservées dans le fichier d'historique. */
/**/
/*     HISTIGNORE : Permet de définir un motif pour exclure des commandes de l'historique (ex: export HISTIGNORE="ls:cd:exit"). */


int delete_n_history_node(t_hist **history, char *options)
{
  char *splitted;
  t_hist *tmp;

  splitted = ft_split(optionsm, ' ');
  if (!splitted)
    return (-1);
  if (splitted[0] && splitted[1] && is_valid_n(history, ft_atoi(splitted[1])))
  {
    n = ft_atoi(splitted[1]);
    if (n == 0))
    {
      free_array(splitted);
      return (0); // 0 ?
    }
    tmp = *history
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
}

  //    history -c : Efface tout l'historique en mémoire (mais pas le fichier ~/.bash_history).
/*     history -d N : Supprime la commande à la ligne N de l'historique. */
/*     history -a : Écrit immédiatement les nouvelles commandes dans le fichier d'historique (~/.bash_history). */
/*     history -w : Écrase le fichier d'historique avec l'historique actuel en mémoire. */
/*     history -r : Recharge l'historique depuis le fichier ~/.bash_history. */
//si retourne 2 : on change le bouleen dans main
int execute_history_options(t_var **env, t_hist **history, char *options)
{
  if (is_valid_option)
  {
    //usage
    return (1);
  }
  if (ft_strchr(options, "c"))
    return (free_history(history));
  else if (ft_strchr(options, "d"))
    return (delete_n_history_node(history, options));
  else if (ft_strchr("a"))
    return (2);
  else if (ft_strchr("w"))
    return (save_history(history, env));
  else if (ft_strchr("r"))
  {
    if (history)
      free_history(history);
    return (read_history(env, history));
  }
  return (1); // on devrait pas arriver la
}

int builtin_history(t_hist **history, char *cmd_line, char **arg)
{
  if (arg[0] && !arg[1])
    return (print_history(history);
  if (is_integer(arg[1]))
    return (print_n_last_cmds(history, ft_atoi(arg[1])));
  return (execute_history_options(hist, arg[1]));
}


int main (void)
{
  t_hist *history;
  exit_code;

  history = NULL;
  exit_code = read_history(&history);
  if (exit_code < 0)
  {
    //malloc
  }
  if (exit_code == 1)
  {
    //fichier sans perm ou erreur ouverture
  }
  //si non on a un historique
  //readline
  //if (is_an_historique_cmd)
  //  {
    //  expand history
  //  }
  //  ici on split et on check si on a une cmd history
  // if (!ft_strcmp(line, "history", 8))
  // {
  //    exit_code = builtin_history(&history, arg);
  // }
  // //on retourne a readline
}
