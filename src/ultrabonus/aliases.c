/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:27:48 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/23 21:55:25 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include "utils.h"
#include "libft.h"
#include <stdlib.h>

char **build_new_array(char **array_to_update, char *alias_value, int alias_size, int line_size)
{
  char **new_array;
  char **alias_array;
  int i;
  int j;

  new_array = malloc(sizeof(char *) * (alias_size + line_size + 1));
  if (!new_array)
    return (NULL);
  alias_array = ft_split(alias_value, ' ');
  i = 0;
  while (alias_array[i])
  {
    new_array[i] = ft_strdup(alias_array[i]);
    i++;
  }
  j = 1;
  while (array_to_update[j])
  {
    new_array[i] = array_to_update[j];
    i++;
    j++;
  }
  new_array[i] = NULL;
  free_array(alias_array);
  return (new_array);
}

int count_words(char *s, char sep)
{
  int i;
  int words;
  int in_word;

  i = 0;
  words = 0;
  in_word = 0;
  while(s && s[i])
  {
    if (s[i] != sep)
      in_word = 1;
    if (in_word == 1 && s[i] == sep)
    {
      in_word = 0;
      words++;
    }
    i++;
  }
  return (words);
}

char *join_new_line(char **array)
{
  int i;
  char *s;
  char *tmp;

  i = 0;
  s = NULL;
  while(array[i])
  {
    tmp = s;
    s = ft_strjoin(s, array[i]);
    free(tmp);
    if (array[i + 1])
    {
      tmp = s;
      s = ft_strjoin(s, " ");
      free(tmp);
    }
    i++;
  }
  return (s);
}

char *expand_alias(char **content, t_alias **alias)
{
  int line_size;
  char *line;
  int alias_size;
  char **new_array;

  line_size = array_size(content);
  alias_size = count_words((*alias)->content, ' ');
  new_array = build_new_array(content, (*alias)->content, alias_size, line_size);
  line = join_new_line(new_array);
  free_array(new_array);
  return (line);
}

t_alias *is_a_known_alias(char *word, t_alias **aliases)
{
  t_alias *tmp;
  
  tmp = *aliases;
  if (!aliases || !*aliases)
    return (NULL);
  while (tmp)
  {
    if (!ft_strncmp(word, tmp->name, ft_strlen(word) + 1))
      return (tmp);
    tmp = tmp->next;
  }
  return (NULL);
}

int exec_alias(t_tree **ast, t_lists *lists, t_alias *alias)
{
  char *line;
  char **strings_env;
  t_tree *tree_to_free;
  int exit_code;

	line = expand_alias((*ast)->token->content, &alias);	
	strings_env = lst_to_array(lists->env);
	tree_to_free = parse(line, *lists->env, lists);
	exit_code = exec_ast(&tree_to_free, lists);
  free_tree(&tree_to_free); 
  free_array(strings_env);
  free(line);
  tree_to_free = NULL;
  return (exit_code);
}

