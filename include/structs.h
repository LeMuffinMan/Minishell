/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:00:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 13:48:17 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "tree.h"
# include <sys/wait.h>

//
# include "history.h"
typedef struct s_hist	t_hist;

//

typedef struct s_alias
{
	// ULTRABONUS
	char				*name;
	char				*content;
	struct s_alias		*next;
}						t_alias;

typedef struct s_shell_fct
{
	// ULTRABONUS
	char				*name;
	char				**content;
	struct s_shell_fct	*next;
}						t_shell_fct;

typedef struct s_var
{
	char				*key;
	char				*value;
	int					exported;
	int					env;
	int					loaded;
	struct s_var		*next;
}						t_var;

typedef struct s_pipe
{
	int					fd[2];
	struct s_pipe		*next;
	struct s_pipe		*prev;
}						t_pipe;

typedef struct s_lists
{
	t_var				**env;
	t_tree				**ast;
	t_pipe				**pipes;
	int					origin_fds[2];
	int					pipe_fd[2];
	int					exit_code;
	// ULTRABONUS
	t_hist				**history;
	t_alias				**aliases;
	t_shell_fct			**shell_fcts;
	// ULTRABONUS
}						t_lists;

typedef struct s_prompt
{
	// ULTRABONUS
	char				*ps1;
	char				*user;
	char				*uid;
	char				*hostname;
	char				*hostname_long;
	char				*pwd;
	char				*git_branch;
	char				*user_type;
	int					total_len;
	char				*prompt;
}						t_prompt;

#endif
