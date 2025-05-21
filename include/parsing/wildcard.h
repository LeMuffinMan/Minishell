/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:04:26 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 22:49:08 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

#include <stdbool.h>

# define INFIX 1
# define PREFIX 2
# define SUFFIX 3
# define JOKER 4

struct					s_token;
typedef struct s_token	t_token;

bool					is_wildcard(char *str);
void					handle_wildcard(t_token **head, bool flag);

#endif