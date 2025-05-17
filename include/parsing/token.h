/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:26:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/17 16:13:32 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define SUCCESS 0
# define NO_F_OR_D 2
# define MEM_ALLOC 12
# define PERMISSION_DENIED 126
# define CMD_NOT_FOUND 127
# define QUOTE 130
# define LITERAL_EXPAND 200

# include <stdbool.h>

struct					s_token;
typedef struct s_token	t_token;
struct					s_var;
typedef struct s_var	t_var;

typedef enum e_type
{
	NO_TOKEN,
	APPEND,
	BUILT_IN,
	CMD,
	DIREC,
	D_QUOTE,
	EXPAND,
	FLE,
	GROUP_PARENTHESIS,
	GROUP_BOOLOP,
	HD,
	O_AND,
	O_OR,
	L_PARENTHESIS,
	R_PARENTHESIS,
	PIPE,
	R_IN,
	S_QUOTE,
	SPACE,
	TRUNC,
	WILDCARD
}	t_type;

/* ----------------------Token---------------------- */
void					assign_token(t_token **head, char **envp,
							t_var *list_env, bool flag);
void					concat_args(t_token **head, t_var *list_env,
							char **envp);
void					delete_space_node(t_token **head);
void					delete_space_node(t_token **head);

/* ------------------Handle command----------------- */
bool					parse_path_without_env(t_token *node);
char					*extract_path(char **envp);
bool					verif_access_exec(char *tmp_cmd, int *error);
char					**split_the_path(char *path);
char					*parse_cmd(char *arg, char **path,
							int *error, bool flag);
char					*verif_command(t_token **node, char *tmp,
							char **path, char **envp);
void					is_built_in(t_token **node);
void					alloc_cmd_split(char ***split_cmd, char **path,
							char *arg, int *error);
void					is_command_whithout_env(t_token **node, char **envp);
void					is_command(t_token **node, char **envp, bool flag);
void					handle_cmd(t_token **node, char **envp, bool flag);
bool					is_valid_argcmd(t_token *node);
void					del_last_space_for_arg(t_token **node, char **tmp);
void					handle_is_command(t_token *node, char *cmd_w_path, bool flag);
int						is_valid_prev(t_token *prev);

int						env_is_alive(char **envp);
void					replace_tab(t_token **node, char *str);

void					check_syntax_error(t_token **head);

t_token					*add_new_token(char *str, int error_code);

#endif
