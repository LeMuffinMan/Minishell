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
#include "utils.h"
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

int	compare_keys(char *key1, char *key2)
{
	char	*longest_key;

	if (ft_strlen(key1) > ft_strlen(key2))
		longest_key = key1;
	else
		longest_key = key2;
	return (ft_strncmp(key1, key2, ft_strlen(longest_key)));
}

int	sort_list(t_var **l)
{
	t_var	*tmp;
	int		sorted;

	sorted = 0;
	tmp = *l;
	if (!tmp)
		return (1);
	while (sorted == 0)
	{
		sorted = 1;
		tmp = *l;
		while (tmp->next)
		{
			if (compare_keys(tmp->key, tmp->next->key) > 0)
			{
				sorted = 0;
				swap_nodes(tmp, tmp->next);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

int	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}

void free_lists(t_lists *lists)
{
    if (!lists)
        return;

    if (lists->env)
    {
        free_list(lists->env);
        free(lists->env);
    }

    if (lists->history)
    {
        free_history(lists->history);
        free(lists->history);
    }

    if (lists->ast)
    {
        if (*lists->ast)
        {
            t_tree *tree_to_free = *lists->ast;
            free_tree(&tree_to_free);
        }
        free(lists->ast);
    }

    if (lists->pipes)
        free(lists->pipes);

    if (lists->aliases)
        free(lists->aliases);

    if (lists->shell_fcts)
        free(lists->shell_fcts);

    free(lists);
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
