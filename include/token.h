/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:26:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 05:19:10 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define SUCCESS 0
# define MEM_ALLOC 12
# define PERMISSION_DENIED 126
# define CMD_NOT_FOUND 127

# include <stdbool.h>

struct s_parse;
typedef	struct s_parse t_parse;

typedef enum e_token
{
	APPEND,
	BUILT_IN,
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
}	t_token;

/* ----------------------Token---------------------- */
void	assign_token(t_parse **head, char **envp);
void	concat_args(t_parse **head);

/* ------------------Handle command----------------- */
bool	parse_path_without_env(t_parse *node);
char	*extract_path(char **envp);
bool	verif_access_exec(char *tmp_cmd, int *error);
char	*verif_path(char **path, char *cmd, int *error);
char	**split_the_path(char *path);
char	*parse_cmd(char *arg, char **path, int *error);
char	*verif_command(t_parse **node, char *tmp, char **path, char **envp);
void	is_built_in(t_parse **node);

int		is_slash(const char *s1);
int		env_is_alive(char **envp);
void	replace_tab(t_parse **node, char *str);

#endif