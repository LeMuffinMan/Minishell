/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:28:16 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/02 22:12:21 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

struct					s_token;
typedef struct s_token t_token;

struct					s_var;
typedef struct s_var t_var;

#include <stdbool.h>

char					*alloc_first_expand(char *value, char *str_to_expand, int index);
char					*ft_strjoin3(char *str_node, char *value, int j, int index);
void					copy_value(t_token *node, char *value, char **value_cpy);
void					to_expand(t_token **node, t_var *list_env);
bool					init_expand(t_token **head, t_var *list_env);
void					add_space(t_token **node);

#endif