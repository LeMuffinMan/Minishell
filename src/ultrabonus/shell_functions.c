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

char **get_shell_function(char *word, t_fct **shell_functions)
{
  t_fct *tmp;

  tmp = *shell_functions
  while(tmp)
  {
    if (!ft_strncmp(word, tmp->name, ft_strlen(tmp->name) + 1))
      return (tmp->content);
    tmp = tmp->next;
  }
  return (NULL);
}

int exec_expanded_content(char **expanded_fct_content, t_var **env, t_fct **shell_functions)
{
  int i;
  t_tree *command_line;

  i = 0;
  while(expanded_fct_content[i])
  {
    command_line = parse(expanded_fct_content[i]);
    exec_ast(&command_line, env, shell_functions);
    //free_tree(command_line);
    command_line = NULL;
    i++;
  }
  return (0);
}

char *expand_var(int start, char *line, char *value)
{
  //si arg[0] : retourner quoi ? ? a gerer avant
  char *new_line;
  int i;

  new_line = malloc(sizeof(char) * ft_strlen(line) - 1 + value);
  if (!new_line)
  {

  }
  ft_strlcpy(new_line, line, start);
  ft_strlcpy((*new_line + start), value, ft_strlen(value)); //possible qu'il faille mettre un +1 pour le \0
  ft_strlcpy((*new_line + start + ft_strlen(value), *line + start)); //meme chose, et est-ce que la valeur de retour de ft_strlcpy permet de faire plus propre ?
  return (new_line); //le pointeur aura avance ?
}

char *expand_line(char *args, char *line)
{
  int i;
  char *new_line;
  char *tmp;

  i = 0;
  while (line[i])
  {
    if (line[i] == '$')
    {
      if (isalnum(line[i + 1]))
      {
        tmp = new_line;
        new_line = expand_var(i, line, args[line[i + 1] - '0']);
        free(tmp);
      }
    }
    i++;
  }
  return (new_line);
}

char **expand_functions_args(char **args, char **shell_fct_content)
{
  char **expanded_fct_content;
  int i;

  expanded_fct_content = malloc(sizeof(char *) * array_size(shell_fct_content) + 1);
  i = 0;
  while (shell_fct_content[i])
  {
    expanded_fct_content[i] = expand_line(args, shell_fct_content[i]);
    i++;
  }
  expanded_fct_content[i] = '\0';
  return (expanded_fct_content);
}

int exec_shell_fct(t_tree **ast, t_var **env, t_fct **shell_functions)
{
  int i;
  t_fct *tmp;
  char **expanded_fct_content;

  command_line = NULL;
  tmp = *shell_functions;
  while (tmp)
  {
    if (ft_strncmp((*ast)->token->content[0], tmp->name))
      break;
    tmp = tmp->next;
  }
  expanded_fct_content = expand_functions_args(tmp->token->content, tmp->content);
  return (0);
}

int main (void)
{
  char **is_shell_function;

  is_shell_function = get_shell_function((*ast)->token->token->content[0])
  if (!is_shell_function)
    return (1); 
  return (0);
}


