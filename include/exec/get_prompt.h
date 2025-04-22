/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:36:46 by oelleaum          #+#    #+#             */
/*   Updated: 2025/04/20 16:03:34 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_PROMPT_H
# define GET_PROMPT_H

#include "structs.h"

//get_prompt.c
char *get_prompt (t_var **env);

// get_prompt_free.c
int			free_prompt(t_prompt *prompt);

// git_branch_utils.c
char		*exctract_branch(char *path_to_head);
char		*trim_git_branch(char *s);
char		*get_branch(char *pwd);

// prompt_gets_fcts.c
char		*get_ps1(t_var **env);
char		*get_user(void);
char		*get_hostname(int mode);
char		*get_uidline(void);
char		*ft_getuid(void);

// prompt_gets_fcts_utils.c
int			get_value_len(t_var **env, t_prompt *prompt, char c);
int			get_prompt_len(char *s, int size, t_prompt *prompt, t_var **env);
int			extract_uid(char *line);
char		*trim_hostname(char *s, int mode);
char		*tilde_replace(char *s, t_var **env);

#endif
