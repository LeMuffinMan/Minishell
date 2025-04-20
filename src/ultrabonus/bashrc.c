#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h> // compiler avec -l readline
#include <fcntl.h>

// Virer les ' ?
// Virer les tab et espaces des fct ?
// Reconnaitre certaines commandes
// mettre GNL
// integrer source ?
//
// Alias :
// ->exec : regenerer un char** pour les arg
//
// Var : 
// revoir le double = check ?
// ajouter a l'env
//
// Cmd :
// seulement built in ?
//
// fct 
// expands $1
// env var
// quotes
// cmd substitution

#define BUFFER_SIZE 1024

typedef struct s_minishellrc 
{
  char *line;
  struct s_minishellrc *next;
} t_minishellrc;

/* typedef struct s_var */
/* { */
/*   char *key; */
/*   char *value; */
/*   struct s_var *next; */
/* } t_var; */

typedef struct s_fct
{
    char *name;
    char **content;
  struct s_fct *next;
} t_fct;

int free_array_minishell_rc(char **array)
{
    int i;

    i = 0;
    while (array && array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
    array = NULL;
}

void add_first_node_minishellrc(t_minishellrc **lst, t_minishellrc *new, char *line)
{
    new->line = ft_strdup(line);
    new->next = NULL;
    *lst = new;
}

void add_back_minishellrc(t_minishellrc **lst, char *line) 
{
    t_minishellrc *ptr;
    t_minishellrc *new;

    new = malloc(sizeof(t_minishellrc));
    if (new == NULL)
    {
        exit(139);
    }
    new->line = NULL;
    new->next = NULL;
    
    if (*lst == NULL)
    {
        add_first_node_minishellrc(lst, new, line);
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

void free_minishellrc(t_minishellrc **minishellrc)
{
    t_minishellrc *current = *minishellrc;
    t_minishellrc *next;

    while (current != NULL)
    {
        next = current->next;
        if (current->line)
            free(current->line);
        free(current);
        current = next;
    }
    *minishellrc = NULL;
}

int print_minishellrc(t_minishellrc **minishellrc)
{
    t_minishellrc *tmp;

    tmp = *minishellrc;
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

void	add_back_var_minishellrc(t_var **lst, char *s) 
{
	t_var	*ptr;
	t_var	*new;
	char **key_value;
	char *cleaned_s;

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
        cleaned_s = ft_strtrim(s, "\n");
		key_value = ft_split(cleaned_s, '=');
		free(cleaned_s);
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

int add_back_fct(t_minishellrc **minishellrc, t_fct **shell_fct, char *name)
{
    t_minishellrc *tmp;
    t_fct *temp;
    t_fct *new_node;
    int i;

    if (!minishellrc || !*minishellrc)
        return (1);

    // Count lines until '}'
    i = 0;
    tmp = *minishellrc;
    while(tmp && !ft_strchr(tmp->line, '}'))
    {
        i++;
        tmp = tmp->next;
    }
    if (!tmp) // No closing brace found
        return (1);

    // Create new node
    new_node = malloc(sizeof(t_fct));
    if (!new_node)
        return (1);
    new_node->content = malloc(sizeof(char *) * (i + 1)); // Fixed allocation size
    if (!new_node->content)
    {
        free(new_node);
        return (1);
    }

    // Initialize new node
    new_node->name = ft_strdup(name);
    new_node->next = NULL;

    // Copy content
    tmp = *minishellrc;
    if (ft_strchr(tmp->line, '{'))
        tmp = tmp->next;
    i = 0;
    while(tmp && !ft_strchr(tmp->line, '}'))
    {
        new_node->content[i] = ft_strdup(tmp->line);
        i++;
        tmp = tmp->next;
    }
    new_node->content[i] = NULL;

    // Add to list
    if (!*shell_fct)
    {
        *shell_fct = new_node;
    }
    else 
    {
        temp = *shell_fct;
        while (temp->next) // Fixed: stop at last node
            temp = temp->next;
        temp->next = new_node;
    }

    return (0);
}

//on cherche un char suivit de ()
//puis on cherche un { : il ne doit pas y avoir de char ASCII entre les deux
//Puis on cherche un }
//on a trouve si tmp n'est pas null
int is_a_shell_function(char *s)
{
    int i;
    int parenthesis;

    parenthesis = 0;
    i = 0;
    while (s && s[i])
    {
        if (s[i] == '(' && i != 0 && ft_isalpha(s[i - 1]))
            parenthesis++;
        if (s[i] == ')' && parenthesis == 1)
            return (1);
        i++;
    }
    return (0);
}

int free_fct(t_fct **fct)
{
    t_fct *tmp;
    t_fct *next;

    tmp = *fct; 
    while (tmp)
    {
        free(tmp->name);
        free_array_minishell_rc(tmp->content);
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    fct = NULL;
}

void print_fct(t_fct **fct)
{
    t_fct *tmp;
    int i;
    
    if (!fct || !*fct)  // Check if pointer is NULL or points to NULL
        return;

    tmp = *fct;
    while (tmp)
    {
        if (tmp->name)  // Check if name exists
            ft_putstr_fd(tmp->name, 1);
            ft_putstr_fd("\n", 1);
        
        if (tmp->content)  // Check if content array exists
        {
            i = 0;
            while (tmp->content[i])  // Check if current string exists
            {
                if (tmp->content[i])  // Additional safety check
                    ft_putstr_fd(tmp->content[i], 1);
                i++;
            }
        }
        tmp = tmp->next;
        ft_putstr_fd("\n", 1);
    }
}

int parse_minishellrc(t_minishellrc **minishellrc)
{
  t_minishellrc *tmp;
  t_var *alias;
  t_var *var;
  char *ps1;
  t_fct *fct;
    char **name;
    char *s;
  /* t_cmd_line *t_cmd_line; */

  ps1 = NULL;
  tmp = *minishellrc;
  alias = NULL;
  var = NULL;
  fct = NULL;
  while (tmp)
  {
    if (tmp->line && tmp->line[0] != '#')
    {
      if (!ft_strncmp(tmp->line, "alias ", 6))
        add_back_var_minishellrc(&alias, tmp->line);
      else if (!ft_strncmp(tmp->line, "PS1", 4))
        {
            s = ft_strtrim(tmp->line, "\n");
            ps1 = ft_strdup(ft_strrchr(s, '='));
            free(s);
        }
      else if (is_a_variable(tmp->line))
            {
                s = ft_strtrim(tmp->line, "\n");
                add_back_var_minishellrc(&var, s);
                free(s);
            }
      else if (is_a_shell_function(tmp->line))
      {
        name = ft_split(tmp->line, '(');
        if(ft_strchr(tmp->line, '{'))
            tmp = tmp->next;
        else 
        {
            while(ft_strchr(tmp->line, '{'))
                tmp = tmp->next;
            tmp = tmp->next;
        }
        add_back_fct(&tmp, &fct, name[0]);
        free_array_minishell_rc(name);
        while (!ft_strchr(tmp->line, '}'))
            tmp = tmp->next;
        tmp = tmp->next;
      }
    }
        if (tmp)
    tmp = tmp->next;
  }
  ft_putstr_fd("---alias---\n", 1);
  print_var(&alias);
  ft_putstr_fd("-----------\n", 1);
  ft_putstr_fd("---var---\n", 1);
  print_var(&var);
  ft_putstr_fd("-----------\n", 1);
  ft_putstr_fd("---fct---\n", 1);
  print_fct(&fct);
  ft_putstr_fd("-----------\n", 1);
  free_var(&alias);
  free_var(&var);
  free_fct(&fct);
}

int load_minishellrc(void)
{
    int fd;
    char buffer[BUFFER_SIZE + 1];
    char *line;
    t_minishellrc *minishellrc;
    char *temp;
    ssize_t bytes_read;
    size_t line_len;
    char *new_line;
    int i;

    minishellrc = NULL;
    line = NULL;
    line_len = 0;
    fd = open(".minishellrc", O_RDONLY);
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
                    free_minishellrc(&minishellrc);
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
                add_back_minishellrc(&minishellrc, new_line);
                free(new_line);
                line_len = 0;
            }
            else
            {
                temp = malloc(line_len + 1);
                if (temp == NULL)
                {
                    free(line);
                    free_minishellrc(&minishellrc);
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
            free_minishellrc(&minishellrc);
            close(fd);
            exit(139);
        }
        line[line_len] = '\0';
        add_back_minishellrc(&minishellrc, line);
        free(line);
    }
    
    close(fd);
    /* print_minishellrc(&minishellrc); */
    parse_minishellrc(&minishellrc);
    free_minishellrc(&minishellrc);
    return (0);
}


int main(void)
{
    load_minishellrc();
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
