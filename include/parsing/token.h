/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:26:57 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/04 18:11:00 by asinsard         ###   ########lyon.fr   */
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
struct					s_lists;
typedef struct s_lists	t_lists;

typedef enum e_type
{
	ARG,
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
bool					concat_args(t_token **head, t_var *list_env,
							bool flag, t_lists *lists);
bool					join_token(t_token **head);
bool					lexing_and_tokenize(char *line, t_token **token,
							t_var *list_env, bool *flag);
bool					parse_again(t_token **head, t_var *list_env,
							bool *flag);
char					**join_content(t_token *node, char **old, char **new);
t_token					*add_new_token(char *str, int error_code, bool flag);
void					assign_token(t_token **head, t_var *list_env,
							bool flag);
void					handle_change_node(t_token **node, bool flag);
void					delete_space_node(t_token **head);
void					delete_space_node(t_token **head);

/* ------------------Handle Command----------------- */
bool					parse_path_without_env(t_token *node);
bool					verif_access_exec(char *tmp_cmd, int *error);
bool					is_valid_argcmd(t_token *node);
char					*extract_path(t_var *list_env);
char					*parse_cmd(char *arg, char **path,
							int *error, bool flag);
char					**split_the_path(char *path);
char					*verif_command(t_token **node, char *tmp,
							char **path, t_var *list_env);
void					alloc_cmd_split(char ***split_cmd, char **path,
							char *arg, int *error);
void					change_node(t_token **node, t_token *next_node,
							char **new_content, bool flag);
void					change_redir(t_token **head);
void					handle_cmd(t_token **node, t_var *list_env, bool flag);
void					handle_is_command(t_token *node,
							char *cmd_w_path, bool flag);
void					is_built_in(t_token **node);
void					is_command(t_token **node, t_var *list_env, bool flag);
void					is_command_whithout_env(t_token **node,
							t_var *list_env);
bool					is_same_family(t_token *node);
bool					is_same_family_redir(t_token *node);

/* --------------------Token Utils------------------- */
bool					env_is_alive(t_var *list_env);
bool					is_pwd_valid(t_var *list_env, t_token *node);
bool					is_valid_prev(t_token *prev);
bool					verif_is_token_valid(t_type token);
char					**copy_tab(t_token *node,
							char **dest, char **src, int index);
void					add_space(t_token **node);
void					del_last_space_for_arg(t_token **node, char **tmp);
void					replace_tab(t_token **node, char *str);

/* --------------------Syntax Error------------------ */
bool					case_is_boolop(t_token *tmp, t_token **head);
bool					case_is_left_parenthesis(t_token *tmp, t_token **head);
bool					case_is_pipe(t_token *tmp, t_token **head);
bool					case_is_right_parenthesis(t_token *tmp, t_token **head);
bool					error_one_parenthesis(t_token **head);
t_token					*set_parenthesis_error(t_token *node);
t_token					*set_syntax_error(t_token *node);
void					alloc_operator_for_error(t_type token, char **operator);
void					case_of_directory_error(t_token **node);
void					check_syntax_error(t_token **head);

#endif
