/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:28:24 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:28:24 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

void	delete_loaded_variables(t_var **env, void (*del)(void *))
{
  t_var *tmp;

  tmp = *env;
  *env = (*env)->next;
  tmp->next = tmp->next->next;
	(*del)((*env)->key);
	(*del)((*env)->value);
	free(*env);
	return (0);
}

int builtin_source(t_var **env, char *path)
{
  t_var *tmp;

  tmp = *env;
  while (tmp)
  {
    if (tmp->next->loaded == 1)
      delete_loaded_variables(tmp, &free)
    tmp = tmp->next;
  }
  load_minishellrc(env, path);
  return (0);
}
