#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <fcntl.h>

#define BUFFER_SIZE 1024

typedef struct s_bashrc 
{
  char *line;
  struct s_bashrc *next;
} t_bashrc;

typedef struct s_var
{
  char *key;
  char *value;
  struct s_var *next;
} t_var;

typedef struct s_fct
{
  char *cmd_line;
  struct s_var var; // je peux mettre t_var ici ?
  struct s_fct *next;
} t_fct;

void add_first_node_bashrc(t_bashrc **lst, t_bashrc *new, char *line)
{
    new->line = ft_strdup(line);
    new->next = NULL;
    *lst = new;
}

void add_back_bashrc(t_bashrc **lst, char *line) 
{
    t_bashrc *ptr;
    t_bashrc *new;

    new = malloc(sizeof(t_bashrc));
    if (new == NULL)
    {
        exit(139);
    }
    new->line = NULL;
    new->next = NULL;
    
    if (*lst == NULL)
    {
        add_first_node_bashrc(lst, new, line);
    }
    else
    {
        ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
        new->line = ft_strdup(line);
    }
}

void free_bashrc(t_bashrc **bashrc)
{
    t_bashrc *current = *bashrc;
    t_bashrc *next;

    while (current != NULL)
    {
        next = current->next;
        if (current->line)
            free(current->line);
        free(current);
        current = next;
    }
    *bashrc = NULL;
}

int print_bashrc(t_bashrc **bashrc)
{
    t_bashrc *tmp;

    tmp = *bashrc;
    while (tmp)
    {
        if (tmp->line)  // Vérifier que line n'est pas NULL
            ft_putstr_fd(tmp->line, 1);
        tmp = tmp->next;
    }
    return (0);  // Ajout d'une valeur de retour
}

void	add_first_node_var(t_var **lst, t_var *new, char *s)
{
	char **key_value;

  if (!ft_strncmp(s, "alias ", 6))
    ft_memmove(s, s + 6, ft_strlen(s) - 5); 
	key_value = ft_split(s, '=');
	new->key = ft_strdup(key_value[0]);
	if (key_value[1])
		new->value = ft_strdup(key_value[1]);
	ft_free(key_value);
	*lst = new;
	new->next = NULL;
}

void	add_back_var(t_var **lst, char *s) 
{
	t_var	*ptr;
	t_var	*new;
	char **key_value;

	new = malloc(sizeof(t_var));
	if (new == NULL)
	{
		/* free_list(lst); */
		exit(139);
	}
	if (*lst == NULL)
		add_first_node_var(lst, new, s);
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->next = NULL;
    if (!ft_strncmp(s, "alias ", 6))
      ft_memmove(s, s + 6, ft_strlen(s) - 5); 
		key_value = ft_split(s, '=');
		new->key = ft_strdup(key_value[0]);
		if (key_value[1])
			new->value = ft_strdup(key_value[1]);
		ft_free(key_value);
	}
}

int is_a_variable(char *s)
{
  int i;

  i = 0;
  while (s && s[i])
  {
    if (s[i] == '=')
      return (1);
    i++;
  }
  return (0);
}

void	free_var(t_var **l)
{
	t_var	*tmp;
	t_var	*next_node;

	tmp = *l;
	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->key)
		  free(tmp->key);
		if (tmp->value)
		  free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
	*l = NULL;
}

int print_var(t_var **var)
{
  t_var *tmp;

  tmp = *var;
  while (tmp)
  {
    ft_putstr_fd(tmp->key, 1);
    ft_putstr_fd("=", 1);
    ft_putstr_fd(tmp->value, 1);
    ft_putstr_fd("\n", 1);
    tmp = tmp->next;
  }
}

/* int add_back_fct(t_fct **fct) */
/* { */
/*   while (!find_closed_bracket(s)) */
/*   { */
/*     while (s[i] && s[i] == ' ') */
/*       i++; */
/*     if (s[i]) */
/*     { */
/*       //on tokenise les variables */
/*         //pour plus loin : si on a un $( : on cree un t_var dedie */
/*       //on tokenise les cmdlines */
/*       //utiliser le parser normal  */
/*     } */
/*     tmp = tmp->next; */
/*   } */
  //tant qu'on n'a pas de }
    //Tant qu'on n'a que des espaces on skip
    //si fin de line, on tmp->next
    //cmd_line = split(line + (spaces nb), ' ')
    //add_back_cmd_line(cmd_line)
  //si on a un }
  //on recupere la ligne sans le }
/*   return (0); */
/* } */

/* int is_a_shell_function(char *s) */
/* { */
/*   int i; */
/*   int parenthesis; */
/**/
/*   i = 0; */
/*   parenthesis = 1; */
/*   while (s && s[i]) */
/*   { */
/*     if (s[i] == '(') */
/*       parenthesis--; */
/*     if (s[i] == ')' && parenthesis == 0) */
/*       return (1); */
/*     i++; */
/*   } */
/*   return (0); */
/* } */

int parse_bashrc(t_bashrc **bashrc)
{
  t_bashrc *tmp;
  t_var *alias;
  t_var *var;
  char *ps1;
  /* t_fct *fct; */
  /* t_cmd_line *t_cmd_line; */

  ps1 = NULL;
  tmp = *bashrc;
  alias = NULL;
  var = NULL;
  while (tmp)
  {
    if (tmp->line && tmp->line[0] != '#')
    {
      if (!ft_strncmp(tmp->line, "alias ", 6))
        add_back_var(&alias, tmp->line);
      else if (!ft_strncmp(tmp->line, "PS1", 4))
        ps1 = ft_strdup(ft_strrchr(tmp->line, '='));
      else if (is_a_variable(tmp->line))
        add_back_var(&var, tmp->line);
      /* else if (is_a_shell_function(tmp->line)) */
      /* { */
        /* add_back_fct(&fct, tmp); */
        /* tant qu'on n'a pas de } on tmp->next */
      /* } */
    }
    tmp = tmp->next;
  }
  ft_putstr_fd("alias : \n", 1);
  print_var(&alias);
  ft_putstr_fd("var : \n", 1);
  print_var(&var);
  free_var(&alias);
  free_var(&var);
}

int load_bashrc(void)
{
    int fd;
    char buffer[BUFFER_SIZE + 1];
    char *line;
    t_bashrc *bashrc;
    char *temp;
    ssize_t bytes_read;
    size_t line_len;
    char *new_line;
    int i;

    bashrc = NULL;
    line = NULL;
    line_len = 0;
    fd = open(".bashrc", O_RDONLY);
    if (fd < 0)
        return (-1);
    
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    while (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        i = 0;
        while (i < bytes_read)
        {
            if (buffer[i] == '\n')
            {
                new_line = malloc(line_len + 2);  // +2 pour le caractère et le '\0'
                if (new_line == NULL)
                {
                    free(line);
                    free_bashrc(&bashrc);
                    close(fd);
                    exit(139);
                }
                if (line_len > 0 && line != NULL)
                {
                    ft_memcpy(new_line, line, line_len);
                    free(line);
                    line = NULL;
                }
                new_line[line_len] = '\n';
                new_line[line_len + 1] = '\0';
                add_back_bashrc(&bashrc, new_line);
                free(new_line);
                line_len = 0;
            }
            else
            {
                temp = malloc(line_len + 1);
                if (temp == NULL)
                {
                    free(line);
                    free_bashrc(&bashrc);
                    close(fd);
                    exit(139);
                }
                if (line_len > 0 && line != NULL)
                {
                    ft_memcpy(temp, line, line_len);
                    free(line);
                }
                line = temp;
                line[line_len] = buffer[i];
                line_len++;
            }
            i++;
        }
        bytes_read = read(fd, buffer, BUFFER_SIZE);
    }
    
    if (line_len > 0)
    {
        line = realloc(line, line_len + 1);
        if (line == NULL)
        {
            free_bashrc(&bashrc);
            close(fd);
            exit(139);
        }
        line[line_len] = '\0';
        add_back_bashrc(&bashrc, line);
        free(line);
    }
    
    close(fd);
    /* print_bashrc(&bashrc); */
    parse_bashrc(&bashrc);
    free_bashrc(&bashrc);
    return (0);
}


int main(void)
{
    load_bashrc();
    return (0);
}

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
