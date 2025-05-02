/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/28 13:29:17 by asinsard         ###   ########lyon.fr   */
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
#include "prints.h"

/* int exec_sequence(char *sequence, t_var **env) */
/* { */
/*     t_tree *seq; */
/*     int exit_code; */
/* 	int origin_fds[2]; */
/**/
/* 	origin_fds[0] = dup(STDIN_FILENO); */
/* 	origin_fds[1] = dup(STDOUT_FILENO); */
/*     seq = NULL; */
/*     strings_env = lst_to_array(&new_env); */
/*     seq = parse(sequence, strings_env); */
/*     free_array(strings_env); */
/*     strings_env = NULL; */
/*     exit_code = exec_seq(&seq, env, origin_fds); */
/*     //update la variable exit_code dans l'environnement ! */
/*     //ici on update tout l'environnement ? */
/*     dup2(origin_fds[0], STDIN_FILENO); */
/*     dup2(origin_fds[1], STDOUT_FILENO); */
/*     close(origin_fds[0]); */
/*     close(origin_fds[1]); */
/*     if (seq) */
/*     { */
/*         free_parse(seq->token, NULL, 0); */
/*         free_tree(&seq); */
/*     } */
/*     return(exit_code) */
/* } */
/**/
/* int get_subshell(t_tree **left, t_tree **right, t_var **env) */
/* { */
/*     pid_t pid; */
/**/
/*     pid = fork(); */
/*     if (pid == -1) */
/*         return(-1); */
/*     if (pid == 0) */
/*     { */
/*         //sur un maillon () la suite est a droite ou a gauche ou les deux ??? */
/*         if (left) */
/*             exit(exec_line(left, env)); */
/*         if (right) */
/*             exit(exec_line(right, env)); */
/*     } */
/*     else */
/*     { */
/*         exit_code = wait_childen(pid, pid); */
/*         return (exit_code); */
/*     } */
/* } */
/**/
/* int exec_line(t_tree **seq_order, t_var **env) */
/* { */
/*     int exit_code; */
/*     t_tree **left; */
/*     t_tree **right; */
/**/
/**/
/*     left = (*seq_order)->left; */
/*     right = (*seq_order)->right; */
/*     exit_code = exec_line(&left, env); */
/*     if ((*seq_order)->token->token == O_AND && exit_code == 0) */
/*         return (exec_line(&right, env)); */
/*     if ((*seq_order)->token->token == O_OR && exit_code != 0) */
/*         return(exec_line(&right, env)); */
/*     if ((*seq_order)->token->token == PARENTHESIS) */
/*         return (get_subshell(&left, &right, env)); */
/*     if (!(*seq_order)->token->token) //si on n'a pas de token : on doit executer */
/*         return (exec_sequence((*seq_order)->token->content[0], env)); //penser aux erreurs de syntaxe avec rien apres ou avant un && ou un || ou deux && || colles */
/*     else //cas ou la porte n'est pas franchie et ou on n'est pas sur un node parenthesis */
/*         return(exit_code); */
/* } */

//exit code $?
//variable _ a gerer dans la liste et pour les echo $_
//CTRL V + Tab : fait un tab dans le minishell a gerer !

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    int        error_code;
    t_var    *new_env;
    t_tree *ast;
    char **strings_env;
	int origin_fds[2];

    /* if (isatty(1)) */
    /* { */
    /*     ft_putstr_fd("Minishell does not support non-interactive mode\n"); */
    /*     exit(0); */
    /* } */
    // pour ac et av : est-ce qu'on veut accepter des demarrages custom ?
    (void)ac;
    (void)av;
    error_code = 0;
    new_env = NULL;
    ast = NULL;
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av);
    /* print_env(&new_env); */

    /* if (find_minishellrc) */
    /*     load_minishell_rc(&new_env); */

	/* dprintf(2, "origin_fds[0] = %d\n", origin_fds[0]); */
	/* dprintf(2, "origin_fds[1] = %d\n", origin_fds[1]); */
    while (1)
    {
        //update env variables !!! si on a un && ou un || on DOIT update l'env entre les deux !!!
        //update prompt
        prompt = NULL;
        /* if (isatty(0) && *env) */
        /* { */
        /*     if (prompt) */
        /*         free(prompt); */
        /*     prompt = get_prompt(&new_env); */
        /* } */
        if (!prompt)
            prompt = "[Minishell]$ ";
        line = ft_strdup("");
        while (ft_strlen(line) == 0 || !ft_strncmp(line, "\n", 2))
        {
            free(line);
            line = NULL;
            line = readline(prompt);
            if (!line)
            {
                free_tree(&ast); // pas de free parse ici ?
                free_list(&new_env);
                exit (error_code);
            }
        }
	    origin_fds[0] = dup(STDIN_FILENO);
	    origin_fds[1] = dup(STDOUT_FILENO);
        /* if (isatty(0) && *env) */
        /* { */
        /*     free(prompt); */
        /*     prompt = NULL; */
        /* } */

        /* t_tree *seq_order; */
        /**/
        /* seq_order = NULL; */
        /* seq_order = get_sequence_order(line); */
        /* error_code = exec_line(&seq_order); */
        /* if (seq_order) // a revoir */
        /* { */
        /*     free_parse(seq_order->token, NULL, 0); */
        /*     free_tree(&seq_order); */
        /* } */
        //
        //
        //
        strings_env = lst_to_array(&new_env);
        ast = parse(line, strings_env);
        free_array(strings_env);
        strings_env = NULL;
        error_code = exec_ast(&ast, &new_env, origin_fds);
        //update la variable exit_code dans l'environnement !
        dup2(origin_fds[0], STDIN_FILENO);
        dup2(origin_fds[1], STDOUT_FILENO);
        close(origin_fds[0]);
        close(origin_fds[1]);
        if (ast)
        {
            free_parse(ast->token, NULL, 0);
            free_tree(&ast);
        }
        //
        //
        //
        free(line);
        line = NULL;
    }
    free_list(&new_env);
    exit(error_code);
}

