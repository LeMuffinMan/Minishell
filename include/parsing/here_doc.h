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

/* -------------HERE DOC FORK AND SIGNALS-------------- */
pid_t					manage_here_doc_fork(int fd, t_lists *lists,
							char *limiter, t_token *node);
int						open_and_fill_here_docs(t_token *tmp, bool sig_hd,
							t_lists *lists, t_token **head);
bool					create_here_doc(t_token *node, t_lists *lists,
							bool *sig_hd);
int						here_doc_readline_signals_handler(char **line,
							char *limiter, int fd);

#endif
