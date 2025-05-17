/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:07:56 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/17 13:05:42 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_H
# define TREE_H

# include "list.h"

struct					s_token;
typedef struct s_token	t_token;

# define PRIO_O_OR 0
# define PRIO_O_AND 0
# define PRIO_PIPE 2
# define PRIO_PARENTHESIS 3
# define PRIO_REDIR 4
# define PRIO_CMD 5
# define PRIO_IGNORE 10

typedef struct s_tree
{
	struct s_tree	*head;
	struct s_tree	*left;
	struct s_tree	*right;
	t_token			*token;
	int				priority;
}	t_tree;

void					add_to_root(t_token **node, t_tree **root);
void					free_tree(t_tree **head);

t_token					*find_best_priority(t_token *start, t_token *end);
void					handle_parenthesis(t_token **node);
void					last_node(t_token **node);
void					assign_priority(t_token **head);
int						handle_boolop_group(t_tree **root);

#endif
