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

// init.c
int		init(t_lists *lists, char **av, char **env);

// init_lists.c
int		init_lists(t_lists *lists);

// init_utils.c
int		init_and_incremente_shlvl(char *s, t_var **env);
int		incremente_shlvl(char *s, char *line);
int		get_cwd_init(char *s);

// misc.c
void	free_lists(t_lists *lists);

//add_back_var.c
int	free_node_var(t_var *node, char **array);

//init_env_utils.c
char **ft_split_on_first_equal(char *s);
int	add_first_node(t_var **lst, t_var **new, char *s, int mode);
int	set_node(t_var **node, int mode);


#endif
