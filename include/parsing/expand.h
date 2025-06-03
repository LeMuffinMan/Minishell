/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:28:16 by asinsard          #+#    #+#             */
/*   Updated: 2025/06/03 16:27:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include <stdbool.h>
# include "structs.h"

struct					s_token;
typedef struct s_token	t_token;

struct					s_var;
typedef struct s_var	t_var;

/* ----------------------EXPAND---------------------- */
char					*alloc_first_expand(char *value,
							char *str_to_expand, int index);

/* -------------------EXPAND UTILS------------------- */
bool					init_expand(t_token **head, t_var *list_env,
							t_lists *lists);
bool					is_valid_arg(t_token *node, char *str);
char					*ft_strjoin3(char *str_node, char *value,
							int j, int index);
void					copy_value(t_token *node, char *value,
							char **value_cpy);
void					delete_first_space(t_token **node);
void					delete_node_pointer(t_token **node);
void					delete_space_content(t_token **node);
void					handle_space(t_token **head);

#endif
