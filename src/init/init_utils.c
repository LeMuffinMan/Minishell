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

void    set_node(t_var **node, int mode)
{
    (*node)->env = 0;
    (*node)->exported = 0;
    (*node)->alias = 0;
    (*node)->shell_fct = 0;
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
}

void	add_first_node(t_var **lst, t_var **new, char *s, int mode)
{
	char	**key_value;

	(*new)->key = NULL;
	(*new)->value = NULL;
	key_value = ft_split(s, '=');
	(*new)->key = ft_strdup(key_value[0]);
	if (key_value[1])
		(*new)->value = ft_strdup(key_value[1]);
	free_array(key_value);
	*lst = *new;
	(*new)->next = NULL;
	set_node(new, mode);
}

// 0 = aucun des deux / 1 = env / 2 = export / 3 = env + export
// revoir le retour d'erreur
void	add_back_var(t_var **lst, char *s, int mode)
{
	t_var	*ptr;
	t_var	*new;
	char	**key_value;

	new = malloc(sizeof(t_var));
	if (new == NULL)
	{
		exit(139);
	}
	if (*lst == NULL)
		add_first_node(lst, &new, s, mode);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->next = NULL;
		/* Value non init avant */
		new->value = NULL;
		new->key = NULL;
		key_value = ft_split(s, '=');
		new->key = ft_strdup(key_value[0]);
		if (key_value && key_value[1])
			new->value = ft_strdup(key_value[1]);
		free_array(key_value);
		set_node(&new, mode);
		/* #include <stdio.h> */
		/* printf("%s=%s\n", new->key, new->value); */
	}
}
