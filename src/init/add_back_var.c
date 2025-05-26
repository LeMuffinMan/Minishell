
#include <stdlib.h>
#include <errno.h>
#include "structs.h"
#include "utils.h"
#include "init.h"
#include "libft.h"

int	free_node_var(t_var *node, char **array)
{
	if (array)
		free_array(array);
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (-1);
}

static int	new_node_allocation(t_var **new)
{
	*new = malloc(sizeof(t_var));
	if (*new == NULL)
		return (-1);
	(*new)->next = NULL;
	(*new)->value = NULL;
	(*new)->key = NULL;
	return (0);
}

static int	process_key_value(t_var *new, char *s)
{
	char	**key_value;

	key_value = ft_split_on_first_equal(s);
	if (errno == ENOMEM)
		return (free_node_var(new, NULL));
	new->key = ft_strdup(key_value[0]);
	if (!new->key)
		return (free_node_var(new, key_value));
	if (key_value && key_value[1])
	{
		new->value = ft_strdup(key_value[1]);
		if (!new->value)
			return (free_node_var(new, key_value));
	}
	free_array(key_value);
	return (0);
}

static int	add_node_to_list(t_var **lst, t_var *new, char *s, int mode)
{
	t_var	*ptr;

	if (!lst || *lst == NULL)
		return (add_first_node(lst, &new, s, mode));
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	return (0);
}

int	add_back_var(t_var **lst, char *s, int mode)
{
	t_var	*new;
	int		ret;

	if (new_node_allocation(&new) == -1)
		return (-1);
	ret = add_node_to_list(lst, new, s, mode);
	if (ret != 0)
		return (ret);
	ret = process_key_value(new, s);
	if (ret != 0)
		return (ret);
	return (set_node(&new, mode));
}
