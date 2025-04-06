/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:38:25 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/01 17:38:51 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_token
{
  APPEND,
  CMD,
  D_QUOTE,
  HD,
  LIM,
  O_AND,
  O_OR,
  PIPE,
  R_IN,
  S_QUOTE,
  TRUNC,
  WILDCARD
} t_token;

typedef struct s_parse
{
  struct s_parse *next;
  struct s_parse *prev;
  char **content;
} t_parse;

typedef struct s_env
{
  char *line;
  // int exported; //booleen ?
  // int local; //pour le cas de VAR=var puis export VAR
  // un booleen pour garder une variable invisible ? : PWD doit marcher avec echo $PWD meme si elle n'est pas dans env ou export
  struct s_env *prev;
  struct s_env *next;
} t_env;

int exec (char **arg, t_env **env);
void	add_back(t_env **lst, char *s);
void	add_first_node(t_env **lst, t_env *new, char *s);
void	free_list(t_env **l);

//utils/builtins_utils
char *build_line(char *s, ...);
int sort_env(t_env **env);

//builtins
int	builtin_cd(char **arg, t_env **env);
int	builtin_echo(char **arg);
int builtin_env(t_env **env);
int	builtin_exit(char **arg, t_env **env);
int builtin_export(t_env **env, char **arg);
int	builtin_pwd(void);
int builtin_unset(t_env **env, char **arg);

#endif
