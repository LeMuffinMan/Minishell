/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:19:04 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/18 22:57:17 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_cont;

	new_lst = NULL;
	new_cont = malloc(sizeof(t_list));
	if (!lst || !new_cont || !f)
		return (NULL);
	new_cont->content = f(lst->content);
	new_lst = new_cont;
	while (lst->next)
	{
		new_lst->next = malloc(sizeof(t_list));
		if (!new_lst->next)
		{
			ft_lstclear(&new_cont, del);
			return (NULL);
		}
		new_lst = new_lst->next;
		lst = lst->next;
		new_lst->content = f(lst->content);
	}
	new_lst->next = NULL;
	return (new_cont);
}
