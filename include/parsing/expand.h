/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:28:16 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/29 17:28:45 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

struct					s_token;
typedef struct s_token t_token;

struct					s_var;
typedef struct s_var t_var;


void	init_expand(t_token **head, t_var *list_env);

#endif