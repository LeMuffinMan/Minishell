#include "token.h"
#include "tree.h"
#include "libft.h"

static void	len_boolop_group(t_tree *ast_node, int *len)
{
	int	i;

	if (!ast_node || !ast_node->token || !ast_node->token->content)
		return ;
	i = 0;
	if (ast_node->left)
		len_boolop_group(ast_node->left, len);
	if (ast_node->right)
		len_boolop_group(ast_node->right, len);
	while (ast_node->token->content[i])
	{
		*len += ft_strlen(ast_node->token->content[i]) + 1;
		i++;
	}
}
static void	copy_boolop_group_content(t_tree *ast_node, char **res, int *pos)
{
	int		i;
	int 	word_len;
	
	if (!ast_node || !ast_node->token || !ast_node->token->content)
		return ;
	i = -1;
	if (ast_node->left)
		copy_boolop_group_content(ast_node->left, res, pos);
	while (ast_node->token->content[++i])
	{
		word_len = ft_strlen(ast_node->token->content[i]);
		ft_memcpy(*res + *pos, ast_node->token->content[i], word_len);
		*pos += word_len;
		(*res)[(*pos)++] = ' ';
	}
	if (ast_node->right)
		copy_boolop_group_content(ast_node->right, res, pos);
}

static void	free_partial_tree(t_tree *node)
{
	if (!node)
		return;
	free_partial_tree(node->left);
	free_partial_tree(node->right);
	if (node->token)
	{
		free_tab(node->token->content);
		free(node->token);
	}
	free(node);
}

#include <stdio.h>
static int	set_boolop_group(t_tree **ast_node, t_token *token_node)
{
	int		len;
	int		pos;
	char	*res;
	t_token	*new_token;
	
	len = 0;
	pos = 0;
	len_boolop_group(*ast_node, &len);
	res = ft_calloc(sizeof(char), len + 1);
	if (!res)
		return (-1);
	copy_boolop_group_content(*ast_node, &res, &pos);
	if (pos > 0 && res[pos - 1] == ' ')
		res[pos - 1] = '\0';
	new_token = add_new_token(res, SUCCESS);
	if (!new_token)
		return (-1);
	free_tab((*ast_node)->token->content);
	free((*ast_node)->token);
	(*ast_node)->token = new_token;
	(*ast_node)->token->group = NULL;
	// if (token_node->next)
	// {
	// 	token_node->next->prev = new_token;
	// 	new_token->next = 
	// }
	(void)token_node;
	(*ast_node)->token->token = GROUP_BOOLOP;
	free_partial_tree((*ast_node)->left);
	free_partial_tree((*ast_node)->right);
	(*ast_node)->left = NULL;
	(*ast_node)->right = NULL;
	free(res);
	return (0);
}

// static int	set_boolop_group(t_tree **ast_node, t_type current_token)
// {
// 	int		len = 0;
// 	int		pos = 0;
// 	char	*res = NULL;
// 	t_token	*new_token = NULL;

// 	(void)current_token;
// 	len_boolop_group(*ast_node, &len);
// 	res = ft_calloc(sizeof(char), len + 1);
// 	if (!res)
// 		return (-1);
// 	copy_boolop_group_content(*ast_node, &res, &pos);
// 	if (pos > 0 && res[pos - 1] == ' ')
// 		res[pos - 1] = '\0';
// 	new_token = add_new_token(res, SUCCESS);
// 	if (!new_token)
// 		return (-1);
// 	free_tab((*ast_node)->token->content);
// 	free((*ast_node)->token);
// 	(*ast_node)->token = new_token;
// 	free_partial_tree((*ast_node)->left);
// 	free_partial_tree((*ast_node)->right);
// 	(*ast_node)->left = NULL;
// 	(*ast_node)->right = NULL;
// 	return (0);
// }

int	handle_boolop_group(t_tree **root)
{
	t_tree	*tmp;

	if (!root || !*root)
		return (0);
	tmp = *root;
	if (tmp->left)
	{
		if ((tmp->token->token == O_AND 
			|| tmp->token->token == O_OR)
				&& tmp->left->token->token != O_AND 
				&& tmp->left->token->token != O_OR
				&& tmp->left->token->token != GROUP_PARENTHESIS)
		{
			if (set_boolop_group(&tmp->left, tmp->token) == -1)
				return (-1);
		}
		else
			handle_boolop_group(&tmp->left);
	}
	if (tmp->right)
	{
		if ((tmp->token->token == O_AND 
			|| tmp->token->token == O_OR)
				&& tmp->right->token->token != O_AND 
				&& tmp->right->token->token != O_OR
				&& tmp->left->token->token != GROUP_PARENTHESIS)
		{
			if (set_boolop_group(&tmp->right, tmp->token) == -1)
				return (-1);
		}
		else
			handle_boolop_group(&tmp->right);
	}
	return (0);
}
