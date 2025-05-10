/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:00:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:22:12 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <sys/wait.h>
# include "tree.h"


//
# include "history.h"
//

typedef struct s_alias
{
	char *name;
	char *content;
	struct s_alias *next;
} t_alias;

typedef struct s_shell_fct
{
	char *name;
	char **content;
	struct s_shell_fcts *next;
} t_shell_fct;

typedef struct s_var
{
	char			*key;
	char			*value;
	int				exported;
	int				env;
	int				alias;
	int				shell_fct;
	int 			loaded;
	struct s_var	*next;
}					t_var;

typedef struct s_pipe
{
	int				fd[2];
	struct s_pipe	*next;
	struct s_pipe	*prev;
}					t_pipe;

typedef struct s_lists
{
	t_var **env;
	t_tree **ast;
	t_pipe **pipes;
	t_hist **history;
	t_alias **aliases;
	t_shell_fct **shell_fcts;
	int origin_fds[2];
} t_lists;

// ultrabonus
// typedef struct s_shell_fct
// {
// 	char *name;
// 	char **content;
// 	struct s_shell_fct *next;
// } t_shell_fct;

// typedef struct s_env
// {
// 	t_var			**heritated_variables;
// 	t_var			**exported_variables;
// 	// t_var **aliases;
// 	// t_fct **shell_fcts;
// 	struct s_env	*next;
// }					t_env;

typedef struct s_prompt
{
	char			*ps1;
	char			*user;
	char			*uid;
	char			*hostname;
	char			*hostname_long;
	char			*pwd;
	char			*git_branch;
	char			*user_type;
	int				total_len;
	char			*prompt;
}					t_prompt;

#endif
