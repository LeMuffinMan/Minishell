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

/* typedef struct s_seq */
/* { */
/*     enum t_type token; // && || et SEQ  */
/*     char *sequence; // ls -l | cat > file1 > file2 | echo "hello '$HOME'" > file3     (NULL si pas en bas d'une branche) */
/*     struct *s_seq left; //comme on a deja pour les ET OU, les parentheses pour changer l'ordre dans l'arbre */
/*     struct *s_seq right; */
/* } t_seq; */
/**/
/**/
/* int exec_seq(t_ast **seq, t_var **env) */
/* { */
/*     t_tree *ast; */
/**/
/*     ast = NULL; */
/*     //tant qu'on a pas la premiere sequence (le node le plus en bas a gauche), on execute de maniere recursive */
/*     if ((*seq)->token == O_AND || (*seq)->token == O_OR) */
/*     { */
/*         //a chaque node && || on fait la comparaison pour continuer a droite ou break et revenir au node parent */
/*         exit_code = exec_seq((*seq)->left) */
/*         if((exit_code != 0 && (*seq)->token == O_AND) || (exit_code == 0 && (*seq)->token == O_OR)) */
/*             return (exit_code); */
/*         else */
/*             return(exec_seq((*seq)->right)); */
/*     } */
/*     if ((*seq)->token == SEQ) */
/*     { */
/*         //une fois en bas de l'arbre, on trouve une seq, on l'envoie a notre parser actuel */
/*         //cette fois on a une line sans aucune parenthese ni && || */
/*         ast = parse((*seq)->sequence); */
/*         exit_code = exec_ast(ast, env); */
/*         //free_tree */
/*     } */
/*     return(exit_code)); */
/* } */
/**/
/* int  new_exec(char *line) */
/* { */
/*     t_seq *seq; */
/*     int exit_code; */
/**/
/*     line = readline(prompt); */
/*     seq = NULL; */
/*     //on recupere un premiere arbre pour connaitre l'ordre des sequences  */
/*     seq = get_sequence_order(line); */
/*     //notre premiere exec cherche la premiere sequence a executer : en bas a gauche */
/*     exit_code = exec_seq(&seq, env); */
/* } */

//TODO ENV
//
//revoir les booleens env | export
//exit code $?
//variable _ a gerer dans la liste et pour les echo $_
//verifier PWD et OLDPWD
//verifier le unset d'une liste vide
//verifier export / export arg args
//verifier env 
//
//CTRL V + Tab : fait un tab dans le minishell a gerer !

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    // char    **arg;
    int        error_code;
    /* int         stdin_fd; */
    /* int         stdout_fd; */
    t_var    *new_env;
    t_tree *ast;
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
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av);
    /* print_env(&new_env); */
    /* if (find_minishellrc) */
    /*     load_minishell_rc(&new_env); */
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
                free_tree(&ast);
                free_list(&new_env);
                exit (error_code);
            }
        }
        /* if (isatty(0) && *env) */
        /* { */
        /*     free(prompt); */
        /*     prompt = NULL; */
        /* } */

        /* new_exec(line); */
        strings_env = lst_to_array(&new_env);
        ast = parse(line, strings_env);
        free_array(strings_env);
        strings_env = NULL;
        error_code = exec_ast(&ast, &new_env);
        if (ast)
        {
            free_parse(ast->token, NULL, 0);
            free_tree(&ast);
        }
        free(line);
        line = NULL;
    }
    free_list(&new_env);
    exit(error_code);
}

