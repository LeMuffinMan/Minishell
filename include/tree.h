/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:07:56 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/09 19:20:17 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_H
# define TREE_H

#include "stack.h"

struct s_token;
typedef struct s_token t_token;

typedef struct s_tree
{
	struct s_tree	*left;
	struct s_tree	*right;
	t_token			*content;
	int				priority;
}	t_tree;



#endif