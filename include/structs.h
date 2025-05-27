/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:00:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/27 17:54:33 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "tree.h"
# include <sys/wait.h>

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
}						t_lists;

#endif
