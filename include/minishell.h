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

int exec (void);

#endif
