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
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdlib.h>
#include <unistd.h>
#include "display.h"

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

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    char    **arg;
    int        error_code;
    t_var    *new_env;
    t_tree *ast;
    /* char **translated_env; */

    /* if (isatty(1)) */
    /* { */
    /*     ft_putstr_fd("Minishell does not support non-interactive mode\n"); */
    /*     exit(0); */
    /* } */
    // pour ac et av : est-ce qu'on veut accepter des demarrages custom ?
    (void)ac;
    (void)av;
    error_code = 1;
    new_env = NULL;
    ast = NULL;
    (void)ast;
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av);
    //execute_minishellrc
    prompt = NULL;
    prompt = get_prompt(&new_env);
    if (!prompt)
        prompt = "[Minishell]$ ";
    while (1)
    {
        //update env variables
        //update prompt
        free(prompt);
        prompt = get_prompt(&new_env);
        if (!prompt)
            prompt = "[Minishell]$ ";
        line = readline(prompt);
        free(prompt);
        prompt = NULL;
        if (line == NULL)
        {
            free(arg);
            free_list(&new_env);
            // a tester !
            exit(error_code);
        }
        arg = ft_split(line, ' ');
        /* translated_env = lst_to_array(&new_env); */
        /* print_array(translated_env); */
    /*     ast = parse(line, translated_env); */
				/* print_ast(ast, 20); */
        error_code = exec(arg, &new_env /* , ast */);
        free(line);
        line = NULL;
        free_array(arg);
        arg = NULL;
        /* free_array(translated_env); */
    }
    free_list(&new_env);
    exit(error_code);
}
