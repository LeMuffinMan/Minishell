/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:43:50 by asinsard          #+#    #+#             */
/*   Updated: 2025/03/26 00:08:42 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFFER_SIZE 10

# define FD_MAX 1024

char	*ft_isnewline(char *s);
char	*ft_strjoin_and_free(char *s1, char *s2);
void	new_memmove(char *buffer, char *storage);
char	*clear_string(char *str, int i);
char	*buf_is_empty(char *buffer, char *storage, int fd, int read_size);
char	*buf_is_full(char *buffer, char *storage, int fd, int read_size);
char	*ft_setline(char *line);
char	*get_next_line(int fd);
char	*copy_and_backspace(char *line, char *str, int i);

#endif
