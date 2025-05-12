/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:04:58 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 19:22:18 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs.h"

void	free_list(t_var **l);
int		sort_list(t_var **l);
int		free_array(char **array);
t_var	*copy_list(t_var **env);
int		print_sorted_env(t_var **env);
int		print_array(char **array);
char	**lst_to_array(t_var **env);
int		free_pipes(t_pipe **pipes);
int		swap_nodes(t_var *n1, t_var *n2);
int close_origin_fds(int origin_fds[2]);
void free_lists(t_lists *lists);
int array_size(char **array);

#endif
