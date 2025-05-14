/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:28:18 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:28:19 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "libft.h"
#include "utils.h"
#include "exec.h"

t_shell_fct *is_a_known_shell_fct(char *word, t_shell_fct **shell_functions)
{
  t_shell_fct *tmp;

  tmp = *shell_functions;
  while(tmp)
  {
    tmp->name[ft_strlen(tmp->name) - 2] = '\0';
    if (!ft_strncmp(word, tmp->name, ft_strlen(tmp->name) + 1))
    {
      tmp->name[ft_strlen(tmp->name)] = '(';
      return (tmp);
    }
    tmp = tmp->next;
  }
  return (NULL);
}

char *expand_var(int start, char *line, char *value)
{
  char *new_line;
  int value_len;
  int line_len;
  int total_len;

  line_len = ft_strlen(line);
  value_len = ft_strlen(value);
  total_len = line_len - 2 + value_len; 
  new_line = malloc(total_len + 1);
  if (!new_line)
      return (NULL);
  ft_strlcpy(new_line, line, start + 1); 
  ft_strlcpy(new_line + start, value, value_len + 1); 
  ft_strlcpy(new_line + start + value_len, line + start + 2, line_len - start - 1);
  return (new_line);
}


int no_arg_to_expand(int j, char **args)
{
  int i;

  i = 0;
  while (args[i] && i < j)
    i++;
  if (i == j && args[i])
    return (0);
  return (1);
}

char *expand_line(char **args, char *line)
{
  int i;
  int j;
  char *new_line;
  char *tmp;

  i = 0;
  new_line = ft_strdup(line);
  while (new_line[i])
  {
    if (new_line[i] == '$' && ft_isdigit(new_line[i + 1]))
    {
      j = new_line[i + 1] - '0';
      if (no_arg_to_expand(j, args))
        tmp = expand_var(i, new_line, "  ");
      else
        tmp = expand_var(i, new_line, args[j]);
      free(new_line);
      new_line = tmp;
      i = -1;
    }
    i++;
  }
  return (new_line);
}

char **expand_functions_args(char **args, char **shell_fct_content)
{
  char **expanded_fct_content;
  int i;

  expanded_fct_content = malloc(sizeof(char *) * (array_size(shell_fct_content) + 1));
  i = 0;
  while (shell_fct_content[i])
  {
    expanded_fct_content[i] = expand_line(args, shell_fct_content[i]);
    i++;
  }
  expanded_fct_content[i] = NULL;

  return (expanded_fct_content);
}

int get_array_size(t_tree **ast, int i)
{
  if ((*ast)->left)
    i += get_array_size(&((*ast)->left), i);
  else if ((*ast)->right)
    i += get_array_size(&((*ast)->right), i);
  return (i + 1);
}

int get_args(t_tree **ast, char **content, int i)
{
  if (i != 0)
    content[i] = ft_strdup((*ast)->token->content[0]); 
  i++;
  if ((*ast)->left)
    return (get_args(&((*ast)->left), content, i));
  if ((*ast)->right)
    return (get_args(&((*ast)->right), content, i));
  return (i);
}

char **gather_function_args(t_tree **ast, char **content)
{
  char **new_array;
  int i;

  i = 0;
  i = get_array_size(ast, i);
  new_array = malloc(sizeof(char *) * (i + 2));
  new_array[0] = ft_strdup(content[0]);
  i = 0;
  i = get_args(ast, new_array, i);
  new_array[i] = NULL;
  return (new_array);
}

int exec_shell_fct(t_tree **ast, t_lists **lists, t_shell_fct *shell_fct)
{
  t_tree *tree_to_free;
  char **strings_env;
  char **expanded_fct_content;
  char **fct_content;
  int i;
  int exit_code;

	fct_content = gather_function_args(ast, (*ast)->token->content);
	expanded_fct_content = expand_functions_args(fct_content, shell_fct->content);	

	i = 0;
	while (expanded_fct_content[i])
	{
		strings_env = lst_to_array((*lists)->env);	
		tree_to_free = parse(expanded_fct_content[i], strings_env, *(*lists)->env);
		exit_code = exec_ast(&tree_to_free, lists);
		free_tree(&tree_to_free);
		tree_to_free = NULL;
		free_array(strings_env);
		i++;
	}
	free_array(expanded_fct_content);
	free_array(fct_content);
	return (exit_code);
}
