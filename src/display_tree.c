/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 23:04:24 by asinsard          #+#    #+#             */
/*   Updated: 2025/04/15 18:25:23 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	print_spaces(int count)
{
	for (int i = 0; i < count; i++)
		printf(" ");
}

void print_ast(t_tree *node, int indent)
{
	if (!node)
		return;
	print_spaces(indent);
	if (node->token && node->token->content && node->token->content[0])
		printf(" %s", node->token->content[0]);
	else
		printf("(null)\n");
	if (node->left || node->right)
	{
		printf("\n");
		print_spaces(indent);
		if (node->left) printf("/");
		if (node->right) printf(" \\\n");
		printf("\n");
		print_ast(node->left, indent - 5);
		print_ast(node->right, indent + 5);
	}
}

void display_ast(t_tree *root)
{
    print_ast(root, 20);  // 20 est l'indent de départ
}


