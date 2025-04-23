#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
/* #include "structs.h" */
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <fcntl.h>

/* #define BUFFER_SIZE 1024 */

typedef struct s_var
{
  char *key;
  char *value;
  struct s_var *next;
} t_var;
/**/
typedef struct s_shell_fct
{
	char *name;
	char **content;
	struct s_shell_fct *next;
} t_shell_fct;

typedef struct s_env
{
  t_var **heritated_variables;
  t_var **exported_variables;
  t_var **aliases;
  t_shell_fct **shell_fcts;
  struct s_env *next;
} t_env;



int	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}
/**/
/* int find_quotes(char *s) */
/* { */
/*     int i; */
/**/
/*     if (!s || s[i] != '\"') */
/*         return (0); */
/*     i = 0; */
/*     while (s[i]) */
/*         i++; */
/*     if (s[i - 1] == '\"') */
/*         return (1); */
/*     else */
/*         return (0); */
/* } */


/**/
/* char **split_alias(char *s) */
/* { */
/*     char **splitted; */
/**/
/*     splitted = ft_split(s, '='); */
/*     if (!splitted) */
/*         return(NULL); */
/*     if (!splitted[1] || !splitted[0] || */
/*         ft_strchr(splitted[0], '\"' || */
/*         ft_strchr(splitted[0], ' '))) */
/*     { */
/*         free_array(splitted); */
/*         return (NULL); */
/*     } */
/*     //si on a des espaces, on veut verifier qu'ils soient bien quoted */
/*     if (ft_strchr(splitted[1], ' ')) */
/*     { */
/*         if (find_quotes(splitted[1]) && !splitted[2]) */
/*             return(splitted); */
/*         else */
/*         { */
/*             free_array(splitted); */
/*             return (NULL); */
/*         } */
/*     } */
/*     else */
/*     { */
/*         if (!splitted[2]) */
/*             return(splitted); */
/*         else */
/*         { */
/*             free_array(splitted); */
/*             return (NULL); */
/*         } */
/*     } */
/* } */

/**/
/* int add_alias(char *line, t_var **aliases) */
/* { */
/*     t_var *new_alias; */
/*     t_var *tmp; */
/*     char *s; */
/*     char **key_value; */
/**/
/*     s = line + 7; */
/*     key_value = split_alias(s); */
/*     new_alias = malloc(sizeof(t_var)); */
/*     if (!new_alias) */
/*         return (-1); */
/*     new_alias->key = key_value[0]; */
/*     new_alias->value = key_value[1]; */
/*     new_alias->next = NULL; */
/*     if (!(*env)->aliases) */
/*         (*env)->aliases = new_alias; */
/*     else */
/*     { */
/*         tmp = *aliases; */
/*         while (tmp->next) */
/*             tmp = tmp->next; */
/*         tmp->next = new_alias; */
/*     } */
/*     return (0); */
/* } */
/**/
/* int add_shell_fct(char **lines, t_var **env, int i, int closed_bracket_index) */
/* { */
/*     t_fct *new_fct; */
/*     t_fct *tmp; */
/*     int j; */
/**/
/*     j = 0; */
/*     //d'abordo on la cherche dans l'env, si on la trouve on change son content */
/*     fct = malloc(sizeof(t_var)); */
/*     fct->name = ft_strdup(lines[i]); */
/*     fct->content = malloc(sizeof(char *) * (closed_bracket_index - i) + 1); */
/*     i += 2; */
/*     while(lines[i] && ft_strlen(lines[i]) != 1 && line[0] != '}') */
/*     { */
/*         fct->content[j] = ft_strdup(lines[i]); */
/*         j++; */
/*         i++; */
/*     } */
/*     fct->content = NULL; */
/*     tmp = (*env)->shell_fct; */
/*     if (!tmp) */
/*         (*env)->shell_fct = new_fct; */
/*     else */
/*     { */
/*         while(tmp->next) */
/*             tmp = tmp->next; */
/*         tmp->next = new_fct; */
/*     }  */
/*     return (0); */
/* } */


/* int is_a_shell_function(char **lines, int start) */
/* { */
/*     int i; */
/*     int j; */
/*     char **splitted; */
/*     char *name; */
/**/
/*     splitted = ft_split(lines[start], "("); */
/*     if (!splitted) */
/*     { */
/*         //error */
/*     } */
/*     if (ft_strlen(splitted[1]) == 1 && splitted[1] == ')' && ft_isalnum(splitted[0])) */
/*         name = ft_strdup(splitted[0]); */
/*     else */
/*         return (0); */
/*     free_array(splitted); */
/*     i = start + 1; */
/*     if (lines[i] && ft_strlen(lines[i]) == 1 && lines[i] == '{' && lines[i + 1] && lines[i + 1][0] != '}') */
/*     { */
/*         i++; */
/*         j = 1; */
/*         while(lines[i] && ft_strlen(line[i]) != 1 && lines[i] == '}') */
/*         { */
/*             j++; */
/*             i++; */
/*         } */
/*         if (!lines[i]) */
/*             return (-1); */
/*         else if (ft_strlen(line[i]) == 1 && lines[i] == '}') */
/*             return (j); */
/*     } */
/*     else */
/*         return (-1); */
/* } */

//tester apres le renew env:
//ajout et parsing de la variable PS1
//ajout des alias
//reconnaissance fct_shell
//ajout des fct shell
//

//on accepte un .minishellrc de 4096 octets max
int load_minishell_rc(t_env **env)
{
    char buffer[4096];
    ssize_t bytes_read;
    char **lines;
    int i;
    int fd;
    int closed_bracket_index;

    fd = open("./.minishellrc", O_RDONLY); //perms ?
    if (fd < 0)
    {
        //error open 
        return (1);
    }
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes_read <= 0)
        return (0);
    buffer[bytes_read] = '\0';
    lines = ft_split(buffer, '\n');
    if (!lines)
    {
        //si on n'a pas de \n
    }
    i = 0;
    while (lines[i])
    {
        /* printf("%s\n", lines[i]); */
        /* closed_bracket_index = is_a_shell_function(lines, i); */
        /* if (closed_bracket_index > 2) */
        /*     add_shell_function(lines, env, i, closed_bracket_index); */
        /* else if (closed_bracket_index < 0) */
        /* { */
        /*     //error syntax */
        /* } */
        if (lines[i][0] && lines[i][0] == '#')
            printf("comment : %s\n", lines[i]);
        else if (!ft_strncmp("alias ", lines[i], 6))
            printf("alias found : %s\n", lines[i]);
        /*     add_alias(lines[i], env); */
        else if (!ft_strncmp("PS1=", lines[i], 4))
        {
            printf("PS1 found : %s\n", lines[i]);
        }
        else //if is command du parsing
            printf("cmd to execute : %s\n", lines[i]);
        /*     exit_code = new_exec(lines[i]); */
        i++;
    }
    free_array(lines);
    return(0);
}

int main(void)
{
    load_minishell_rc(NULL);
    return(0);
}


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
