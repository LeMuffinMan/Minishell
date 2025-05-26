/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:30:41 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:59:55 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "structs.h"

int		init(t_lists *lists, char **av, char **env);
int		init_lists(t_lists *lists);
int		init_and_incremente_shlvl(char *s, t_var **env);
int		incremente_shlvl(char *s, char *line);
int		get_cwd_init(char *s);
void	free_lists(t_lists *lists);
int	free_node_var(t_var *node, char **array);
char **ft_split_on_first_equal(char *s);
int	add_first_node(t_var **lst, t_var **new, char *s, int mode);
int	set_node(t_var **node, int mode);
int	init_lists(t_lists *lists);

#endif
