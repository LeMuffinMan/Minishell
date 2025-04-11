/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/11 17:35:23 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tree.h"
#include "minishell.h"
#include <readline/readline.h> // compiler avec -l readline
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "display.h"

char **lst_to_array(t_var **env)
{
    t_var *tmp;
    char **array;
    char *s;
    char *temp;
    int i;

    i = 0;
    tmp = *env;
    array = NULL;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    array = malloc(sizeof(char *) * (i + 1));
    if (!array)
        return (NULL);
    tmp = *env;
    i = 0;
    while (tmp)
    {
        if (!tmp->value)
            s = ft_strdup(tmp->key);
        else
        {
            s = ft_strdup(tmp->key);
            temp = s;
            s = ft_strjoin(s, "=");
            free(temp);
            temp = s;
            s = ft_strjoin(s, tmp->value);
            free(temp);
        }
        array[i] = s;
        //il FAUT free a la sortie du parser 
        i++;
        tmp = tmp->next;
    }
    array[i] = NULL;
    return (array);
}

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    char    **arg;
    int        error_code;
    t_var    *new_env;
    t_tree *ast;
    char **translated_env;

    /* t_ast *ast; */
    /* if (isatty(1)) */
    /* { */
    /*     ft_putstr_fd("Minishell does not support non-interactive mode\n"); */
    /*     exit(0); */
    /* } */
    (void)ac;
    (void)av;
    // pour ac et av : est-ce qu'on veut accepter des demarrages custom ?
    new_env = NULL;
    prompt = "[Minishell]$ ";
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av);
    while (1)
    {
        line = readline(prompt);
        if (line == NULL)
        {
            free(arg);
            free_list(&new_env);
            // a tester !
            exit(error_code);
        }
        arg = ft_split(line, ' ');
        translated_env = lst_to_array(&new_env);
        ast = parse(line, translated_env);
				print_ast(ast, 20);
        error_code = exec(arg, &new_env /* , ast */);
        free(line);
        line = NULL;
        free_array(arg);
        arg = NULL;
        free_array(translated_env);
    }
    free_list(&new_env);
    exit(error_code);
}
