/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/23 04:09:38 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_prompt.h"
#include "tree.h"
#include "exec.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <stdlib.h>
#include <unistd.h>
#include "display.h"

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
    char **strings_env;

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
    //execute_minishellrc avant ou apres ?
    while (1)
    {
        //update env variables !!! si on a un && ou un || on DOIT update l'env entre les deux !!!
        //update prompt
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
        if (isatty(0) && env)
            free(prompt);
        prompt = NULL;
        strings_env = lst_to_array(&new_env);
        /* print_array(strings_env); */
        ast = parse(line, strings_env);
        free_array(strings_env);
	    /* if (find_here_docs(&ast)) */
		   /*  exec_here_docs(ast); */
		/* display_ast(ast); */
        // error_code = exec_ast(&ast, &new_env, &pipes, &pids);
        free(line);
        line = NULL;
    }
    free_list(&new_env);
    exit(error_code);
}
