/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/17 15:04:55 by oelleaum         ###   ########lyon.fr   */
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

int print_env(t_var **env)
{
    t_var *tmp;

    tmp = *env;
    while (tmp)
    {
        if (tmp->key)
            ft_putstr_fd(tmp->key, 1);
        if (tmp->value) //Si on fait VAR= on ft_strdup("")
        {
            ft_putstr_fd("=", 1);
            ft_putstr_fd(tmp->value, 1);
        }
        ft_putstr_fd("\n", 1);
        tmp = tmp->next;
    }
    return (0);
}

int exec_here_docs(t_tree *ast)
{
    (void)ast;
	//
	return (0);
}

int find_here_docs(t_tree **ast)
{
	if ((*ast)->token->token == HD)
		return (1);
	else if ((*ast)->left)
		return (exec_here_docs((*ast)->left));
	else if ((*ast)->right)
		return (exec_here_docs((*ast)->right));
	return (0);
}

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    // char    **arg;
    int        error_code;
    t_var    *new_env;
    t_tree *ast;
    t_pipe *pipes;
    t_pids *pids;
    char **translated_env;

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
    pipes = NULL;
    pids = NULL;
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av);
    /* print_env(&new_env); */
    //execute_minishellrc
    while (1)
    {
        //update env variables !!! si on a un && ou un || on DOIT update l'env entre les deux !!!
        //update prompt
        //
        prompt = NULL;
        if (isatty(0) && *env)
        {
            if (prompt)
                free(prompt);
            prompt = get_prompt(&new_env);
        }
        if (!prompt)
            prompt = "[Minishell]$ ";
        line = readline(prompt);
        /* printf("%s\n", line); */
        if (line == NULL)
        {
            // free(arg);
            free_list(&new_env);
            exit(error_code);
        }
        if (isatty(0) && env)
            free(prompt);
        prompt = NULL;
        // arg = ft_split(line, ' ');
        translated_env = lst_to_array(&new_env);
        /* print_array(translated_env); */
        ast = parse(line, translated_env);
	    if (find_here_docs(&ast))
		    exec_here_docs(ast);
		/* display_ast(ast); */
        error_code = exec_ast(&ast, &new_env, &pipes, &pids);
        free(line);
        line = NULL;
        // free_array(arg);
        // arg = NULL;
        free_array(translated_env);
    }
    free_list(&new_env);
    exit(error_code);
}
