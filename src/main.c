/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:21:52 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/29 18:48:03 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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

//TODO
//Mardi : merge 
//tester : 
    //pipeline + redirs : fixer leak
    //expands + quotes :
    //builtins (env et export surtout)
    //signaux

//pour le merge suivant (vendredi 9 mai ?):
    //ast pour les && || ()
    //signaux dans les pipes
    //builtins fixed in pipe   
    //here_docs ?
    //echo et export avec expands
    //export VAR="a b"  des cas de con de ouf a gerer export A="    b . bb    .   "
    //normer et securiser tout

//Debuggage industriel (1 semaine ?)

//overkill time + On clean, on norme, on securise TOUT
//final merge sur master pour vendredi 16 mai / semaine du 20 mai ?

//ultrabonus :
    //tester load minishellrc avec le parsing fini
    //proteger get_prompt
    //alias : 
        //tester exec
        //decla d'alias au parsing ?
        //token d'alias au parsing ?
    //shell_fct :
        //decla shlfct au parsing ?
        //token shlfct au parsing ?
    //substitution cmd :
        //exception dans les expands 
        //fct pour executer la cmd_s
    //faire l'historique a la main ?
        //charger au demarrage un .minishell_history
        //save l'historique a la fin, meme avec un ctrl c 
        //limite l'historique a une size (on ecrase la plus ancienne entree)
        //un builtin history qui affiche le contenu du fichier avec le numerotage des lignes 
        //ajouter !! et !-x 

//Tests de cons :
//CTRL V + Tab : fait un tab dans le minishell a gerer !


int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    int        exit_code;
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
    exit_code = 0;
    new_env = NULL;
    ast = NULL;
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(&new_env, env, av[0]);
    /* print_all_variables(&new_env); */
    /* print_env(&new_env); */

    if (find_minishellrc(&new_env, NULL))
        load_minishellrc(&new_env, NULL);
    while (1)
    {
        set_signals(0);
        prompt = NULL;
        if (isatty(0) && *env)
        {
            if (prompt)
                free(prompt);
            prompt = get_prompt(&new_env);
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
                    free(prompt);
                free_tree(&ast); // pas de free parse ici ?
                free_list(&new_env);
                exit (exit_code);
            }
            add_history(line);
            //A gerer avec les signaux correctement !
            //pour avoir un historique complet on est suppose l'enregistrer dans un fichier a la sortie
            //et charger ce fichier au demarrage si il existe
            //on peut fixer une limite a l'historique des commandes 
        }
	    origin_fds[0] = dup(STDIN_FILENO);
	    origin_fds[1] = dup(STDOUT_FILENO);
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
        strings_env = lst_to_array(&new_env);
        ast = parse(line, strings_env, new_env);
        free_array(strings_env);
        strings_env = NULL;
        exit_code = exec_ast(&ast, &new_env, origin_fds);
        update_exit_code_var(&new_env, exit_code);
        //update la variable exit_code dans l'environnement !
        dup2(origin_fds[0], STDIN_FILENO);
        dup2(origin_fds[1], STDOUT_FILENO);
        close(origin_fds[0]);
        close(origin_fds[1]);
        if (ast)
            free_tree(&ast);
        //
        //
        //
        free(line);
        line = NULL;
    }
    free_list(&new_env);
    exit(exit_code);
}

