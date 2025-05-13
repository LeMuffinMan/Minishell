/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:04:05 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 17:29:07 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "utils.h"
#include <stdlib.h>

void	free_list(t_var **l)
{
	t_var	*tmp;
	t_var	*next_node;

	tmp = *l;
	if (!*l)
		return ;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
	*l = NULL;
}

int    set_node(t_var **node, int mode)
{
  (*node)->env = 0;
  (*node)->exported = 0;
  (*node)->loaded = 0;

  if (mode == 1)
  {
      (*node)->env = 1;
  }
  else if (mode == 2)
  {
      (*node)->exported = 1;
  }
  else if (mode == 3)
  {
      (*node)->env = 1;
      (*node)->exported = 1;
  }
  else if (mode == 4)
  {
      (*node)->loaded = 1;
  }
  return (0);
}

int free_node_var(t_var *node, char **array)
{
	if (array)
		free_array(array);
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (-1);
}

int	add_first_node(t_var **lst, t_var **new, char *s, int mode)
{
	char	**key_value;

	(*new)->key = NULL;
	(*new)->value = NULL;
	key_value = ft_split(s, '=');
	if (!key_value)
		return (free_node_var(*new, NULL));
	(*new)->key = ft_strdup(key_value[0]);
	if (!(*new)->key)
		return (free_node_var(*new, key_value));
	if (key_value[1])
	{
		(*new)->value = ft_strdup(key_value[1]);
		if (!(*new)->value)
			return (free_node_var(*new, key_value));
	}
	free_array(key_value);
	*lst = *new;
	(*new)->next = NULL;
	return (set_node(new, mode));
}


// 0 = aucun des deux / 1 = env / 2 = export / 3 = env + export
// revoir le retour d'erreur
int	add_back_var(t_var **lst, char *s, int mode)
{
	t_var	*ptr;
	t_var	*new;
	char	**key_value;

	new = malloc(sizeof(t_var));
	if (new == NULL)
		return (-1);
	if (!lst || *lst == NULL)
		return (add_first_node(lst, &new, s, mode));
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	new->next = NULL;
	new->value = NULL;
	new->key = NULL;
	key_value = ft_split(s, '=');
	if (!key_value)
		return (free_node_var(new, NULL));
	new->key = ft_strdup(key_value[0]);
	if (!new->key)
		return (free_node_var(new, key_value));
	if (key_value && key_value[1])
	{
		new->value = ft_strdup(key_value[1]);
		if (!new->value)
			return (free_node_var(new, key_value));
	}
	free_array(key_value);
	return (set_node(&new, mode));
}
