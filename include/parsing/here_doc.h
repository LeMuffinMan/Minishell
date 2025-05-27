/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:51:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 00:55:39 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# define NAME_LENGTH 10

# include <stdbool.h>
# include "structs.h"

struct					s_token;
typedef struct s_token	t_token;

/* ----------------------HERE DOC---------------------- */
bool					create_hd_name(t_token **head);
bool					handle_here_doc(t_token **head, t_lists *lists);
bool					verif_name(char *name);
int		close_origin_fds(int origin_fds[2]);
void	free_lists(t_lists *lists);
bool	exec_here_doc(t_token *node, t_lists *lists);
bool	extract_stdin(int fd, char *limiter);
bool	create_hd_name(t_token **head);

#endif
