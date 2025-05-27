/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:51:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:39 by oelleaum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# define NAME_LENGTH 10

# include "structs.h"
# include <stdbool.h>

struct					s_token;
typedef struct s_token	t_token;

/* ----------------------HERE DOC---------------------- */
bool					create_hd_name(t_token **head);
bool					exec_here_doc(t_token *node, t_lists *lists);
bool					extract_stdin(int fd, char *limiter);
bool					handle_here_doc(t_token **head, t_lists *lists);

/* -------------------HERE DOC UTILS------------------- */
bool					case_is_limiter(char *line, char *limiter, int fd,
							int len);
bool					free_lim_close_fd(char *limiter, int fd);
bool					verif_here_doc(t_token **head);
bool					verif_name(char *name);
char					*get_limiter(char *s, int fd);
int						close_origin_fds(int origin_fds[2]);
void					free_lists(t_lists *lists);

#endif
