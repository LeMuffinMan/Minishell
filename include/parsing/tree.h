/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:07:56 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/26 16:21:37 by asinsard         ###   ########lyon.fr   */
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
# define PRIO_IGNORE 8
# define IS_EX_VALUE 99

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

void					len_boolop_group(t_tree *ast_node, int *len);
void					copy_boolop_group_content(t_tree *ast_node, char **res, int *pos);
void					free_partial_tree(t_tree *node);

int						find_len_new_content(t_token *node, t_token *end);
void					copy_nodes_content(t_token *start, t_token *end, char **res,
								t_type current_token);
void					free_node_unused(t_token *node);

#endif
