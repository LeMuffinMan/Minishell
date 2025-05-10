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
#include "history.h"

/* int exec_sequence(char *sequence, t_var **env) */
/* { */
/*     t_tree *seq; */
/*     int exit_code; */
/* 	int origin_fds[2]; */
/**/
/* 	origin_fds[0] = dup(STDIN_FILENO); */
/* 	origin_fds[1] = dup(STDOUT_FILENO); */
/*     seq = NULL; */
/*     strings_env = lst_to_array(lists->env); */
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


//Exec todo 
    //Bultins refacto
        //echo : vierer tous les espaces, afficher content[x] colles 
        //export : export a="123" : content[0] export / 1 = a= / 2 = "123"
        //unset SHLVL USER HOME PATH export | grep SHLVL export | grep USER export | grep HOME export | grep PATH : STDERR + EXIT fails
        //echo > /home/muffin/.file : STDERR
        //fix simple cmd : uname -m -n -r -s : STDOUT + EXIT
        //fix redirections : 
            //< log/file_without_permissions whoami
            //< log/infile cat :
            //TOUTES les multiples : errors 

//parsing todo
    //bug fixs
        //cd ..
        //>
    //

//pour le merge de mardi 13 mai
    //exec clean
    //builts refacto
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

//Merge d'apres (20 mai) : debuggage industriel
//->push 


//TODO
//Mardi : merge 
//tester : 
    //signaux : revoir avec builtins ?
    //expands + quotes : echo et export 
    //builtins (env et export surtout)

//pour le merge suivant (vendredi 9 mai ?):
    //ast pour les && || ()
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
        //utils a finir
        //a tester

//Tests de cons :
//CTRL V + Tab : fait un tab dans le minishell a gerer !

int init_lists(t_lists **lists)
{
    *lists = malloc(sizeof(t_lists));
    if (!*lists)
        return (-1);
    
    (*lists)->env = NULL;
    (*lists)->ast = NULL;
    (*lists)->pipes = NULL;
    (*lists)->history = NULL;
    (*lists)->aliases = NULL;
    (*lists)->shell_fcts = NULL;
    (*lists)->env = malloc(sizeof(t_var*));
    if (!(*lists)->env)
    {
        free(*lists);
        return (-1);
    }
    *(*lists)->env = NULL;
    (*lists)->ast = malloc(sizeof(t_tree*));
    if (!(*lists)->ast)
    {
        free((*lists)->env);
        free(*lists);
        return (-1);
    }
    *(*lists)->ast = NULL;
    (*lists)->pipes = malloc(sizeof(t_pipe*));
    if (!(*lists)->pipes)
    {
        free((*lists)->ast);
        free((*lists)->env);
        free(*lists);
        return (-1);
    }
    *(*lists)->pipes = NULL;
    (*lists)->history = malloc(sizeof(t_hist*));
    if (!(*lists)->history)
    {
        free((*lists)->pipes);
        free((*lists)->ast);
        free((*lists)->env);
        free(*lists);
        return (-1);
    }
    *(*lists)->history = NULL;
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

    if (lists->ast && *lists->ast)
    {
        t_tree *tree_to_free = *lists->ast;
        free_tree(&tree_to_free);
        free(lists->ast);
    }

    if (lists->pipes)
        free(lists->pipes);

    free(lists);
}

int main(int ac, char **av, char **env)
{
    char    *line;
    char    *prompt;
    int        exit_code;
    t_lists *lists;
    t_var *new_env;
    t_tree *tree_to_free; 
    /* t_tree *ast; */
    /* t_pipe *pipes; */
    /* t_hist *history; */
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
    lists = NULL;
    if (init_lists(&lists) == -1)
    {
        //malloc error 
    }
    exit_code = 0; //on ajoute l'exit code a la megastruct ou on la laisse dans env ?
    /* utiliser getenv ?
        * Si on n'a pas d'env uniquement ?*/
    init_env(lists->env, env, av[0]);
    /* print_all_variables(lists->env); */
    /* print_env(lists->env); */

    //revoir retour d'erreur
    if (isatty(0) && *env && find_minishellrc(lists->env, NULL))
        load_minishellrc(lists->env, NULL);
    if (isatty(0) && *env)
        load_history(lists->env, lists->history);
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
                if (isatty(0) && env && *env)
                {
                    if (prompt)
                        free(prompt);
                    if (lists->history)
                        save_history(lists->env, lists->history);
                }

                free_lists(lists);
                exit(exit_code);
            }           
            if (ft_strlen(line) > 0)
            {
                if (!lists->history && isatty(0) && *env)
                    ft_add_history(lists->env, lists->history, line);
                else if (isatty(0) && *env && lists->history && *lists->history)
                {
                    if ((*lists->history)->prev == NULL || 
                        ft_strncmp(line, (*lists->history)->prev->cmd_line, ft_strlen(line) + 1))
                        ft_add_history(lists->env, lists->history, line);
                }
                else if (lists->history && *lists->history && 
                        (*lists->history)->prev && 
                        ft_strncmp(line, (*lists->history)->prev->cmd_line, ft_strlen(line) + 1))
                    add_history(line);
            }
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
        new_env = *lists->env;
        strings_env = lst_to_array(lists->env);
        *lists->ast = parse(line, strings_env, *lists->env);
        free_array(strings_env);
        strings_env = NULL;
        exit_code = exec_ast(lists->ast, lists->env, origin_fds, lists->pipes);
        update_exit_code_var(lists->env, exit_code);
        //update la variable exit_code dans l'environnement !
        dup2(origin_fds[0], STDIN_FILENO);
        dup2(origin_fds[1], STDOUT_FILENO);
        close(origin_fds[0]);
        close(origin_fds[1]);
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

