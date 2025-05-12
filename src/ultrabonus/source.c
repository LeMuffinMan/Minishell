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

#include "libft.h"
#include "minishellrc.h"
#include "structs.h"

int	delete_loaded_variables(t_var **env, void (*del)(void *))
{
	(*del)((*env)->key);
	(*del)((*env)->value);
	free(*env);
	return (0);
}

int print_source_error_message()
{
  ft_putstr_fd("minishell: source: filename argument required\nsource: usage: source filename [arguments]\n", 2);
  return (2);
}

int builtin_source(char *arg, t_lists **lists)
{
  t_var *tmp;
  t_var *last;

  /* if (!arg[1]) */
  /*   return (print_source_error_message()); */
  tmp = *((*lists)->env);
  last = NULL;
  while (tmp)
  {
    if (ft_strncmp("PS1", tmp->key, 4))
    {
      if (!last)
      {
        *((*lists)->env) = tmp->next;
        delete_loaded_variables(&tmp, &free);
        tmp = *((*lists)->env);
      }
      else
      {
        last->next = tmp->next;
        delete_loaded_variables(&tmp, &free);
        tmp = last;
      }
    }
    else
    {
      last = tmp;
      tmp = tmp->next;
    }
  }
  load_minishellrc((*lists)->env, (*lists)->aliases, (*lists)->shell_fcts, arg);
  return (0);
}
