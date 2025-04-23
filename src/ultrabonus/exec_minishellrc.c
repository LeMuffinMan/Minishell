#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <fcntl.h>

#define BUFFER_SIZE 1024

int is_a_fct_name(char *line)
{
    int i;
    char *strs;

    i = 0;
    strs = ft_split(line, ')');
    if (strs[1] == ')' && ft_strlen(strs[1] == 1) && strs[0]) //imposer un jeu restreint de char pour nom de fct ?
    {
        free_array(strs);
        return (1);
    }
    else
    {
        free_array(strs);
        return (0);
    }
}

int seek_open_bracket(int fd)
{
    char *line;
    
    line = get_next_line(fd);
    if (ft_strlen(line) == 1 && line[0] == '{')
    {
        free(line);
        return(1);
    }
    else
    {
        free(line);
        return(0);
    }
}

int seek_closed_bracket(int fd)
{
    char *line;
    int count;

    count = 0;
    line = get_next_line(fd);
    while ((line != NULL)
    {
        if (ft_strlen(line) == 1 && line[0] == '}')
        {
            free(line);
            return(count);
        }
        free(line);
        line = get_next_line(fd);
        count++;
    }
    return(0)
}

int get_child_exit_code(pid_t pid)
{
	int		status;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	if (exit_code == EXIT_SUCCESS && WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (exit_code == EXIT_SUCCESS && WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

//name()
//{
//}
//ce cas n'est pas accepte
//renvoie > 0 si on a une fct (le nb de ligne de son contenu)
//renvoie 0 si ce n'est pas une fonction
//renvoie -1 si on a un nom de fct mais un { ou } manquante 
int is_a_shell_function(int fd, char *line)
{
    pid_t pid;
    int exit_code;

    pid = fork();
    if (pid == -1)
    {

    }
    if (pid == 0)
    {
        if (is_a_fct_name(line))
        {
            if (seek_open_bracket(fd))
            {
                exit_code = seek_closed_bracket(fd);
                if (exit_code > 0)
                    exit (exit_code);
                else
                {
                    //error parsing : no closed bracket
                    exit (-1);
                }
            }
            else
            {
                //error parsing : open bracket
                exit (-1);
            }
        }
        else
            exit (0);
    }
    else
        return (get_child_exit_code(pid))
}


int add_shell_fct(t_var **env, char *name, int fd)
{
    char *tmp;
    char *s;

    s = ft_strjoin(name, "=");
    fct = malloc(sizeof(t_var));
    
    //le parent etait sur la ligne du nom, donc on fait deux gnl avant d'avoir la premiere ligne
    line = get_next_line(fd);
    free(line);
    line = get_next_line(fd);
    while(line[0] != '}')
    {
        tmp = s;
        s = ft_strjoin(s, line);
        free(tmp);
        free(line);
        line = get_next_line(fd);
    }
    add_back_var(env, s, SHELL_FCT);
    return (0);
}

//ouvre minishellrc
//execute chaque ligne
//si une ligne est une declaration de fonction
    //envoie un child verifier la syntaxe { }
//si la decla est correcte : on parse le contenu et on ajoute la fct a l'env
//on continue d'executer chaque ligne
int load_minishell_rc(t_var **env)
{
    int fd;
    char *line;
    int line_number;

    fd = open(".minishellrc", O_RDONLY); //perms ?
    if (fd < 0)
    {
        //error open 
        return (1);
    }
    line = get_next_line(fd);
    line_number = 0;
    while ((line != NULL)
    {
        //ajouter un is alias si pas fait au parsing
        fct_content_size = is_a_shell_function(line);
        if (fct_content_size > 0)
            add_shell_fct(env, line, fd);
        else if (fct_content_size == -1)
            parsing_error(line_number + 1); // + 1 parce que la bracket suit le nom de la fct
        else
            new_exec(line);
        free(line);
        line = get_next_line(fd);
        line_number++;
    }
    close(fd);
    return 0;
}

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

typedef struct s_var
{
  char *key;
  char *value;
  //booleen ?
  int env; 
  int exported;
    /* int alias; */
    /* int shell_fct; */
  struct s_var *next;
} t_var;

typedef enum e_mode
{
    ENV,
    EXPORT,
    ALIAS,
    SHELL_FCT
}	t_mode;

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
