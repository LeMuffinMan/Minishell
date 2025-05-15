/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 19:27:02 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "init.h"
#include "misc.h"
#include "get_prompt.h" // A VIRER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "tree.h"
#include "exec.h"
#include "utils.h"
#include "env_utils.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include "display.h"
#include "prints.h"
#include "signals.h"
#include "minishellrc.h"
#include "history.h"

/* int exec_sequence(char *sequence, t_var **env) */
/* { */
/*     t_tree *seq; */
/*     int exit_code; */
/* 	int lists->origin_fds[2]; */
/**/
/* 	lists->origin_fds[0] = dup(STDIN_FILENO); */
/* 	lists->origin_fds[1] = dup(STDOUT_FILENO); */
/*     seq = NULL; */
/*     strings_env = lst_to_array(lists->env); */
/*     seq = parse(sequence, strings_env); */
/*     free_array(strings_env); */
/*     strings_env = NULL; */
/*     exit_code = exec_seq(&seq, env, lists->origin_fds); */
/*     //update la variable exit_code dans l'environnement ! */
/*     //ici on update tout l'environnement ? */
/*     dup2(lists->origin_fds[0], STDIN_FILENO); */
/*     dup2(lists->origin_fds[1], STDOUT_FILENO); */
/*     close(lists->origin_fds[0]); */
/*     close(lists->origin_fds[1]); */
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


//Exec todo 
    //Bultins refacto
        //unset SHLVL USER HOME PATH export | grep SHLVL export | grep USER export | grep HOME export | grep PATH : STDERR + EXIT fails

//pour le merge de mardi 13 mai
    //exec clean
    //builtins refacto
    //()
    //here_docs
    //bug fixs 
    //wildcards
//A tester 
    //la nouvelle exec avec ()
    //builtins 
    //here_docs (signaux !!)
    //wildcards
    //signaux

//Merge d'apres (20 mai) 
    //cd + pwd builtin error to fix
    //debuggage nouvelle exec + ()
    //debuggage Here_docs + quotes + expands
    //debuggage wildcard
    //exec malloc protection
    //normed

//debuggage 1 semaine ?

//Push autour du 26 mai ?

//ici

//ultrabonus :
    //tester load minishellrc avec le parsing fini
    //proteger get_prompt
    //alias : 
        //token d'alias au parsing ?
    //shell_fct :
        //token shlfct au parsing ?
    //history
        //tester builtin
        //tester ! cmds

//Tests de cons :
//CTRL V + Tab : fait un tab dans le minishell a gerer !

//TODO pour merge 20 mai
//exec :
    //integrer nouvelle exec && || () 
    //fixer le builtin exit
        //fixer cd et proteger cd - et getcwd a l'init
        //malloc protection de tout exec.c
        //normage exec.c
//parsing :
    //bug fixes
        //ajouter les fixs en reserve ( $HONE $USER ) + (unset )
    //here_doc (expand dans here_doc ?)
    //wildcards
//A TESTER AU MERGE 
    //la nouvelle exec
    //here_docs
    //wildscards
    //BUILTINS
    //SIGNAUX
//
//TODO POUR LE 26 mai
    //BUG FIXES de ce qui a merde au merge, et des cas de cons 
    //gitaction pour proteger le push sur master
    //norm & malloc protection check
//
//push 26 mai



//est-ce qu'on garde ac ?
int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    int        exit_code;
    t_lists *lists;
    t_tree *tree_to_free; 
    char **strings_env;

    (void)ac;
    //decommenter a la toute fin !!!
    /* is_interactive_mode(); */
    exit_code = 0; //on ajoute l'exit code a la megastruct ou on la laisse dans env ?
    lists = NULL;
    if (init(&lists, av, env) == -1)
    {
        //malloc error
        free_lists(lists);
        return (-1);
    }
    while (1)
    {
        setup_parent_signals();
        prompt = NULL;
        if (isatty(0) && *env)
        {
            if (prompt)
                free(prompt);
            prompt = get_prompt(lists->env);
        }
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
                if (isatty(0) && *env)
                {
                    if (prompt)
                        free(prompt);
                    //A gerer avec les signaux correctement !
                    if (lists->history)
                        save_history(lists->env, lists->history);
                }
                free_lists(lists);
                exit(exit_code);
            }           
            if (ft_strlen(line) > 0 && !is_duplicated_hist_entry(lists->history, line))
            {
                if (isatty(0) && *env)
                    ft_add_history(lists->env, lists->history, line);
                else
                    add_history(line);
            }
        }
	    lists->origin_fds[0] = dup(STDIN_FILENO);
	    lists->origin_fds[1] = dup(STDOUT_FILENO);
        if (isatty(0) && *env)
        {
            free(prompt);
            prompt = NULL;
        }

        /* t_tree *seq_order; */
        /**/
        /* seq_order = NULL; */
        /* seq_order = get_sequence_order(line); */
        /* exit_code = exec_line(&seq_order); */
        /* if (seq_order) // a revoir */
        /* { */
        /*     free_parse(seq_order->token, NULL, 0); */
        /*     free_tree(&seq_order); */
        /* } */
        //
        //
        //
        /* new_env = *lists->env; */
        strings_env = lst_to_array(lists->env);
        *lists->ast = parse(line, strings_env, *lists->env);
        free_array(strings_env);
        strings_env = NULL;
        exit_code = exec_ast(lists->ast, &lists);
        update_exit_code_var(lists->env, exit_code);
        //update la variable exit_code dans l'environnement !
        dup2(lists->origin_fds[0], STDIN_FILENO);
        dup2(lists->origin_fds[1], STDOUT_FILENO);
        close(lists->origin_fds[0]);
        close(lists->origin_fds[1]);
        if (lists->ast && *lists->ast) 
        {
            tree_to_free = *lists->ast; 
            free_tree(&tree_to_free); 
            *lists->ast = NULL;
        }
        //
        //
        //
        free(line);
        line = NULL;
    }
    free_list(lists->env);
    exit(exit_code);
}

