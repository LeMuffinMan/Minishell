/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:38:25 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/08 14:02:27 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum e_var_type 
{
  PTR,
  ARRAY,
  ENV,
  PARSE,
} t_var_type;

typedef void (*t_free_fct)(void*);

typedef struct s_env
{
  char *key;
  char *value;
  //booleen ?
  int env; 
  int exported;
  //si un bool == 1 on veut l'afficher dans env ou export
  //si les deux == 0 : variables de shell, non exportees 
  struct s_env *prev;
  struct s_env *next;
} t_env;

typedef struct s_gar 
{
  void *allocated_memory;
  t_free_fct free_fct;
  struct s_gar *next;
} t_gar;

int exec (char **arg, t_env **env);
void	add_back(t_env **lst, char *s, int mode);
void	add_first_node(t_env **lst, t_env *new, char *s, int mode);
void	free_list(t_env **l);

//utils/builtins_utils
char *build_line(char *s, ...);
int sort_env(t_env **env);
int is_only_numeric_argument(char *s);
int print_env(t_env **env);

//builtins
int	builtin_cd(char **arg, t_env **env);
int	builtin_echo(char **arg);
int builtin_env(t_env **env);
int	builtin_exit(char **arg, t_env **env);
int builtin_export(t_env **env, char **arg);
int	builtin_pwd(void);
int builtin_unset(t_env **env, char **arg);

#endif
