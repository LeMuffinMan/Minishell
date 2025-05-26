/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:51:21 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 19:42:03 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# define NAME_LENGTH 10

# include <stdbool.h>
# include "structs.h"

struct					s_token;
typedef struct s_token	t_token;

bool					handle_here_doc(t_token **head, t_lists *lists);
bool					create_hd_name(t_token **head);
bool					verif_name(char *name);

#endif
