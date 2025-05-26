/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:58:12 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/26 17:58:15 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_UTILS_H
# define CD_UTILS_H

int	change_directory(char *path);
int	builtin_cd_without_arg(t_var **env);
int	find_last_slash(char *buf);
int	trim_pwd(char **s);
int		builtin_pwd(t_var **env);
int		array_size(char **array);

#endif
