/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:49:15 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/23 23:22:03 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

# include "list.h"

# define PB_QUOTE 128

bool					is_quote(t_token **node);
bool					is_to_expand(char *str);
bool					del_last_quote(t_token **node);

#endif