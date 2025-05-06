/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:07:56 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/30 17:09:16 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_H
# define TREE_H

# include "list.h"

struct					s_token;
typedef struct s_token	t_token;

typedef struct s_tree
{
	struct s_tree **head;
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
	t_token			*token;
	int				priority;
}	t_tree;

void					add_to_root(t_token *node, t_tree **root, bool flag);
void					free_tree(t_tree **head);

t_token					*find_best_priority(t_token *start, t_token *end, bool flag);
void					last_node(t_token **node, bool flag);
void					assign_priority(t_token **head, bool flag);
void					set_bool_seq(t_tree **root);

#endif
