/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:00:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:04:12 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include <sys/wait.h>

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

typedef struct s_pids
{
	pid_t pid;
	struct s_pids *next;
} t_pids;

//ultrabonus
typedef struct s_prompt
{
	char	*ps1;
	char	*user;
	char	*uid;
	char	*hostname;
	char	*hostname_long;
	char	*pwd;
	char	*git_branch;
	char	*user_type;
	int		total_len;
	char	*prompt;
}			t_prompt;

#endif
