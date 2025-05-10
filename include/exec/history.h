/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:26:30 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/08 16:57:24 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
#define HISTORY_H

typedef struct s_hist
{
  char *cmd_line;
  struct s_hist *next;
  struct s_hist *prev;
} t_hist;



int ft_add_history(t_var **env, t_hist **history, char *line);
int load_history(t_var **env, t_hist **history);
int free_history(t_hist **history);
int save_history(t_var **env, t_hist **history);


#endif
