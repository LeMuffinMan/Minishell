/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:38:25 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/17 15:18:31 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "tree.h"
#include <sys/wait.h>

// typedef enum e_token
// {
//   APPEND,
//   CMD,
//   D_QUOTE,
//   HD,
//   LIM,
//   O_AND,
//   O_OR,
//   PIPE,
//   R_IN,
//   S_QUOTE,
//   TRUNC,
//   WILDCARD
// } t_token;
//
// typedef struct s_parse
// {
//   struct s_parse *next;
//   struct s_parse *prev;
//   char **content;
// } t_parse;

// typedef enum e_var_type 
// {
//   PTR,
//   ARRAY,
//   ENV,
//   PARSE,
// } t_var_type;
//
// typedef void (*t_free_fct)(void*);

typedef struct s_var
{
  char *key;
  char *value;
  //booleen ?
  int env; 
  int exported;
  //si un bool == 1 on veut l'afficher dans env ou export
  //si les deux == 0 : variables de shell, non exportees 
  struct s_var *next;
} t_var;

typedef struct s_pipe
{
	int fd[2];
	struct s_pipe *next;
	struct s_pipe *prev;
} t_pipe;

typedef struct s_pid
{
	pid_t pid;
	struct s_pids *next;
} t_pid;

typedef struct s_pids
{
	pid_t pid;
	struct s_pids *next;
} t_pids;

typedef struct s_prompt 
{
  char *ps1;  
  char *user;
  char *uid;
  char *hostname;
  char *hostname_long;
  char *pwd;
  char *git_branch;
  char *user_type;
  int total_len;
  char *prompt;
} t_prompt;



// typedef struct s_gar 
// {
//   void *allocated_memory;
//   t_free_fct free_fct;
//   struct s_gar *next;
// } t_gar;

int exec_ast (t_tree **ast, t_var **env, t_pipe **pipes, t_pids **pids);
void	add_back_var(t_var **lst, char *s, int mode);
void	add_first_node(t_var **lst, t_var *new, char *s, int mode);
void	free_list(t_var **l);

//BUILTINS
//builtins/export
int free_array(char **array);
int	compare_keys(char *key1, char *key2);
int builtin_export(t_var **env, char **arg);
//builtins/cd
int	builtin_cd(char **arg, t_var **env);
//builtins/echo
int	builtin_echo(char **arg);
//builtins/env
int builtin_env(t_var **env);
//builtins/exit
int	builtin_exit(char **arg, t_var **env);
//builtins/pwd
int	builtin_pwd(void);
//builtins/unset
int builtin_unset(t_var **env, char **arg);
int env_size(t_var *env);

//init
int	init_env(t_var **new_env, char **env, char **arg);

//UTILS
//utils/builtins_utils
int sort_var(t_var **env);
int is_only_numeric_argument(char *s);
int print_var(t_var **env);
int	array_size(char **array);
t_var	*copy_list(t_var **env);
//utils/env_utils
char	*get_value(t_var **env, char *key);
int	update_env(t_var **env);
//utils/prints.c
int	print_sorted_env(t_var **env);
int	print_array(char **array);

//utils.c 
char **lst_to_array(t_var **env);
t_tree	*parse(char *line, char **envp);


//get_prompt
char *get_prompt (t_var **env);
int free_prompt(t_prompt *prompt);


#endif
