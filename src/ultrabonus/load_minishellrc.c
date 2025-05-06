/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_minishellrc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:28:02 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/24 16:28:04 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "exec.h"
#include "env_utils.h"
#include "utils.h"
#include "structs.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <fcntl.h>
#include <env_utils.h>

/* #define BUFFER_SIZE 1024 */

/* typedef struct s_var */
/* { */
/*   char *key; */
/*   char *value; */
/*     int exported; */
/*     int env; */
/*     int alias; */
/*     int shell_fct; */
/*   struct s_var *next; */
/* } t_var; */
/**/
/* typedef struct s_shell_fct */
/* { */
/* 	char *name; */
/* 	char **content; */
/* 	struct s_shell_fct *next; */
/* } t_shell_fct; */

/* typedef struct s_env */
/* { */
/*   t_var **heritated_variables; */
/*   t_var **exported_variables; */
/*   t_var **aliases; */
/*   t_shell_fct **shell_fcts; */
/*   struct s_env *next; */
/* } t_env; */



int find_quotes(char *s)
{
    int i;

    i = 0;
    if (!s || s[i] != '\'')
        return (0);
    i = 0;
    while (s[i])
        i++;
    if (s[i - 1] == '\'')
        return (1);
    else
        return (0);
}



char **split_alias(char *s)
{
    char **splitted;

    splitted = ft_split(s, '=');
    if (!splitted)
        return(NULL);
    if (!splitted[1] || !splitted[0])
    {
        free_array(splitted);
        return (NULL);
    }
    //si on a des espaces, on veut verifier qu'ils soient bien quoted
    if (ft_strchr(splitted[1], ' '))
    {
        if (find_quotes(splitted[1]) && !splitted[2])
            return(splitted);
        else
        {
            free_array(splitted);
            return (NULL);
        }
    }
    else
    {
        if (!splitted[2])
            return(splitted);
        else
        {
            free_array(splitted);
            return (NULL);
        }
    }
}


int add_alias(char *line, t_var **env)
{
    t_var *new_alias;
    t_var *tmp;
    char *s;
    char **key_value;

    s = line + 6;
    key_value = split_alias(s);
    if (!key_value)
    {
        return (-1);
        //error
    }
    new_alias = malloc(sizeof(t_var));
    if (!new_alias)
        return (-1);
    new_alias->key = ft_strdup(key_value[0]);
    new_alias->value = ft_strdup(key_value[1]);
    free_array(key_value);
    new_alias->next = NULL;
    new_alias->exported = 0;
    new_alias->env = 0;
    new_alias->shell_fct = 0;
    new_alias->alias = 1;
    new_alias->loaded = 1;
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_alias;
    return (0);
}


char *join_fct_content(char **lines, int i, int closed_bracket_index)
{
    char *s;
    char *tmp;

    s = ft_strdup("");
    while(i < closed_bracket_index)
    {
        if (s)
            tmp = s;
        s = ft_strjoin(s, lines[i]);
        if (tmp)
            free(tmp);
        tmp = s;
        s = ft_strjoin(s, "\n");
        free(tmp);
        i++;
    }
    return (s);
}

int setup_fct_node(t_var **new_node, char *line)
{
    (*new_node)->next = NULL;
    (*new_node)->exported = 0;
    (*new_node)->env = 0;
    (*new_node)->alias = 0;
    (*new_node)->shell_fct = 1;
    (*new_node)->loaded = 1;
    (*new_node)->key = ft_strdup(line);
    if (!(*new_node)->key)
    {
        free((*new_node));
        return (-1);
    }
    return (0);
}

int add_shell_fct(char **lines, t_var **env, int i, int closed_bracket_index)
{
    t_var *tmp;
    t_var *new_fct;
    
    new_fct = malloc(sizeof(t_var));
    if (!new_fct)
        return (-1);
    if (setup_fct_node(&new_fct, lines[i]) == -1)
        return (-1);
    i+=2;
    new_fct->value = join_fct_content(lines, i, closed_bracket_index);
    if (!new_fct->value)
    {
        free(new_fct->key);
        free(new_fct);
        return (-1);
    }
    tmp = *env;
    while(tmp->next)
        tmp = tmp->next;
    tmp->next = new_fct;
    return (0); 
}

//ajouter un itoa ?
int print_syntax_error()
{
    ft_putstr_fd("syntax error loading minishellrc line ...", 2);
    return (-1);
}

int find_closed_bracket(char **lines, int i)
{
    i += 2;
    while (lines[i] && ft_strlen(lines[i]) != 1 && lines[i][0] != '}')
        i++;
    if (!lines[i])
        return(print_syntax_error());
    else /* if (ft_strlen(line[i] == 1 && lines[i][0] == '}') */
        return(i);
}
                        

//return 0 si ce n'est pas une declaration de fct : on continue a executer ligne par ligne
//return -1 si ya une erreur de syntaxe : on arrete le load ou un malloc fail ? 
//return i : retourne la position de la bracket fermante
int is_a_shell_function(char **lines, int start)
{
    int i;
    char **splitted;

    splitted = ft_split(lines[start], '(');
    if (!splitted)
        return (-1);
    if (!splitted[1] || splitted[2]) // si le split donne pas 2 mots
    {
        free_array(splitted);
        return (0);
    }
    if (ft_strlen(splitted[0]) >= 1 && splitted[1][0] == ')' && !splitted[1][1]) //et isalnum ?
    {
        free_array(splitted);
        i = start;
        //on a un nom de fonction
        if (lines[i + 1] && ft_strlen(lines[i + 1]) == 1 && lines[i + 1][0] == '{')
            return (find_closed_bracket(lines, i));
        else
            return (print_syntax_error());
    }
    free_array(splitted);
    return (print_syntax_error());
}
    
//tester apres le renew env:
//ajout et parsing de la variable PS1
//ajout des alias
//reconnaissance fct_shell
//ajout des fct shell
//

int print_fct_content(char *name, t_var **env)
{
    t_var *tmp;

    tmp = *env;
    while(tmp)
    {
        if (!ft_strncmp(name, tmp->key, ft_strlen(tmp->key) + 1))
        {
            ft_putstr_fd(tmp->value, 1);
            return (0);
        }
        tmp = tmp->next;
    }
    return (1);
}

char **read_minishell_rc(char *path)
{
    char buffer[4096];
    ssize_t bytes_read;
    int fd;
    char **lines;

    if (!path)
        path = "./.minishellrc";
    fd = open(path, O_RDONLY); //perms ?
    if (fd < 0)
    {
        //error open 
        return (NULL);
    }
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes_read <= 0)
        return (0);
    buffer[bytes_read] = '\0';
    lines = ft_split(buffer, '\n');
    if (!lines)
        return (NULL);
    return (lines);
}

char *trim_quotes(char *str)
{
    int len = ft_strlen(str);
    char *trimmed;
    int i;

    if (len < 2 ||
        !( (str[0] == '\'' && str[len - 1] == '\'') ||
           (str[0] == '\"' && str[len - 1] == '\"') ) )
        return (str);
    trimmed = malloc(sizeof(char) * (len - 1));
    if (!trimmed) 
        return (NULL);
    i = 0;
    while (i < len - 2)
    {
        trimmed[i] = str[i + 1];
        i++;
    }
    trimmed[i] = '\0';
    return trimmed;
}

int is_a_segfault_trap(char *line, char *file_path)
{
    char **path_to_load;
    char *trimmed_line;

    trimmed_line = trim_quotes(line);
    path_to_load = NULL;
    path_to_load = ft_split(trimmed_line, ' ');
    //si on a pas la cmd source
    /* printf("path_to_load[0] = %s | path_to_load[1]= %s | file_path = %s\n", path_to_load[0], path_to_load[1], file_path); */
    if (path_to_load[0] && ft_strncmp(path_to_load[0], "source", 7))
    {
        free_array(path_to_load);
        return (0);
    }
    //si le path donne a source est le meme que le file path 
    /* printf("path_to_load = %s | file_path = %s\n", path_to_load[1], file_path); */
    if (!ft_strncmp(path_to_load[1], file_path, ft_strlen(file_path)))
    {
        /* printf("ERROR MSG !\n"); */
        free_array(path_to_load);
        return (1); //1 pour empecher le segfault / 0 pour le laisser faire
        //qund on pourra executer chaque ligne a tester 
    }
    free_array(path_to_load);
    return (0);
}

int print_noway()
{
    ft_putstr_fd("[STACK OVERFLOW] Bravo. You made .minishellrc self-replicate until reality tore.\nThis isn’t a bug — this is performance art.\nI'm just the canvas. Enjoy your core dump, Banksy.\n", 1);
        return (0);
}

int add_or_update_last_rc_file_var(t_var **env, char *path)
{
    char *value;
    t_var *node;

    value = get_value(env, "last_rc_file");
    if (!value)
    {
        value = ft_strjoin("last_rc_file=", path);
        add_back_var(env, value, 0); // ajouter une exception pour unset
        if (value)
            free(value);
        return (0);
    }
    else
    {
        node = get_key_node(env, "last_rc_file");
        free(node->value);
        node->value = ft_strdup(path);
        return (0);
    }
}

//on accepte un .minishellrc de 4096 octets max
int load_minishellrc(t_var **env, char *path)
{
    char **lines;
    int i;
    int closed_bracket_index;
    t_var *tmp;

    /* printf("path = %s\n", path); */
    lines = read_minishell_rc(path);
    if (!lines)
        return (-1);
    i = 0;
    while (lines[i])
    {
        /* printf("%s\n", lines[i]); */
        closed_bracket_index = is_a_shell_function(lines, i);
        if (closed_bracket_index < 0)
            return (-1);
        if (closed_bracket_index > 2)
        {
            add_shell_fct(lines, env, i, closed_bracket_index);
            /* printf("shell fct name: %s\n", lines[i]); */
            /* print_fct_content(lines[i], env); */
            i = closed_bracket_index;
        }
        /* if (lines[i][0] && lines[i][0] == '#') */
        /* { */
            /* printf("comment : %s\n", lines[i]); */
        /* } */
        else if (!ft_strncmp("alias ", lines[i], 6))
        {
            add_alias(lines[i], env);
            tmp = *env;
            while(tmp->next)
                tmp = tmp->next;
            /* printf("alias added in env : %s=%s\n", tmp->key, tmp->value); */
        }
        else if (!ft_strncmp("PS1=", lines[i], 4))
        {
            add_back_var(env, lines[i], 4);
            tmp = *env;
            while(tmp->next)
                tmp = tmp->next;
            /* printf("PS1 added in env : %s=%s | exported = %d | env = %d\n", tmp->key, tmp->value, tmp->exported, tmp->env); */
        }
        else if (ft_strlen(lines[i]) > 1 && lines[i][0] != '}' && lines[i][0] != '#')//if is command du parsing
        {
            /* printf("lines[%d] = %s\n", i, lines[i]); */
            if (path && is_a_segfault_trap(lines[i], path) > 0)
            {
                if (print_noway())
                {
                    //error
                }
            }
            else
            {
                //execute line 
            /* printf("cmd to execute : %s\n", lines[i]); */
        /*     exit_code = exec_sequence(lines[i], env); */
            }
            (void)lines;
        }
        i++;
    }
    add_or_update_last_rc_file_var(env, path);
    free_array(lines);
    return(0);
}

char *get_default_minishellrc_path(t_var **env)
{
	char	*s;
	char *tmp;

    s = get_value(env, "HOME");
    if (!s)
    {
        s = get_value(env, "USER");
        if (!s)
        {
            ft_putstr_fd("minishell: load_minishellrc: no HOME or USER variable set\n", 2);
            return (NULL);
        }
        tmp = s;
        s = ft_strjoin("/home/", s);
        free(tmp);
    }
    return(s);
}

char *find_minishellrc(t_var **env, char *path)
{

    if (!path)
        path = get_default_minishellrc_path(env);
    if (access(path, F_OK) == -1)
    {
        print_error_file_opening(path, "No such file or directory\n");
        return (NULL);
    }
    if (access(path, R_OK) == 0)
        return (path);
    else 
    {
        ft_putstr_fd("minishell: .minishellrc: Permission denied\n", 2);
        return (NULL);
    }
}

/**/
/* int main(void) */
/* { */
/*     load_minishellrc(NULL); */
/*     return(0); */
/* } */


//ouvre minishellrc
//execute chaque ligne
//si une ligne est une declaration de fonction
    //envoie un child verifier la syntaxe { }
//si la decla est correcte : on parse le contenu et on ajoute la fct a l'env
//on continue d'executer chaque ligne
/* int load_minishell_rc(t_var **env) */
/* { */
/*     int fd; */
/*     char *line; */
/*     int line_number; */
/**/
/*     fd = open(".minishellrc", O_RDONLY); //perms ? */
/*     if (fd < 0) */
/*     { */
/*         //error open  */
/*         return (1); */
/*     } */
/*     line = get_next_line(fd); */
/*     line_number = 0; */
/*     while ((line != NULL) */
/*     { */
/*         //ajouter un is alias si pas fait au parsing */
/*         fct_content_size = is_a_shell_function(line); */
/*         if (fct_content_size > 0) */
/*             add_shell_fct(env, line, fd); */
/*         else if (fct_content_size == -1) */
/*             parsing_error(line_number + 1); // + 1 parce que la bracket suit le nom de la fct */
/*         else */
/*             new_exec(line); */
/*         free(line); */
/*         line = get_next_line(fd); */
/*         line_number++; */
/*     } */
/*     close(fd); */
/*     return 0; */
/* } */
/**/
/* [muffin@ArchZen ~]$ test() { */
/* > echo $HOME */
/* > } */
/* [muffin@ArchZen ~]$ test */
/* /home/muffin */
/* [muffin@ArchZen ~]$  */

/* [muffin@ArchZen /]$ test () { */
/* > cd / */
/* > pwd */
/* > echo $(cd && pwd) */
/* > } */
/* [muffin@ArchZen /]$  */
/* [muffin@ArchZen /]$ test */
/* / */
/* /home/muffin */
/* [muffin@ArchZen /]$  */

//Todo
//pour implementer :
//- ajouter enum mode env
//- update env struct
// Test :
// ouverture du fichier
// absence du fichier
// parsing d'un export
// parsing d'un alias
// parsing d'une fonction

//pour les alias : 
//si cmd == alias
//on rappelle le parser, mais avec le contenu de l'alias a la place de l'alias
//
//pour cd ~
//on ouvre /etc/passwd et on cherche 1000:1000 ou alors on cherche /home/ ?
//pour chaque ligne /home/ fait un split on cherche le champ 6
//on fait un access sur le champ 6 et si c'est non on fait tous les autres
//si oui : c'est le path
//
//variable PS1
//\h The hostname, up to the first ‘.’.
/* \H The hostname. */
/*\n new line*/
/* \s The name of the shell, the basename of $0 */
/* \u  The username of the current user.*/
/* \W The basename of $PWD, with $HOME abbreviated with a tilde */
/* \e exit code */

//foncions shell
//appeler le parser pour recuperer un AST de chaque ligne
//executer chaque AST dans l'ordre
//reprendre l'execution normale
// Gerer $0 $1 -> expand des argv (max 9)
