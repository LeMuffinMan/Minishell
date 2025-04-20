/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:58:12 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 18:01:02 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "libft.h"
#include <stdlib.h>

char **lst_to_array(t_var **env)
{
    t_var *tmp;
    char **array;
    int i;
    int count = 0;
    char *s;
    char *temp;

    tmp = *env;
    while (tmp && ++count)
        tmp = tmp->next;
    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return NULL;
    tmp = *env;
    i = 0;
    while (tmp)
    {
        if (tmp->value)
        {
            s = ft_strjoin(tmp->key, "=");
            temp = s;
            array[i] = ft_strjoin(s, tmp->value);
            free(temp);
        }
        else
            array[i] = ft_strdup(tmp->key);
        
        if (!array[i])
        {
            while (i-- > 0)
                free(array[i]);
            free(array);
            return NULL;
        }
        i++;
        tmp = tmp->next;
    }
    array[i] = NULL;
    return array;
}

/* int exec_here_docs(t_tree *ast) */
/* { */
/*     (void)ast; */
/* 	// */
/* 	return (0); */
/* } */
/**/
/* int find_here_docs(t_tree **ast) */
/* { */
/* 	if ((*ast)->token->token == HD) */
/* 		return (1); */
/* 	else if ((*ast)->left) */
/* 		return (exec_here_docs((*ast)->left)); */
/* 	else if ((*ast)->right) */
/* 		return (exec_here_docs((*ast)->right)); */
/* 	return (0); */
/* } */
