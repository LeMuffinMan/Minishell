/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:20:38 by asinsard          #+#    #+#             */
/*   Updated: 2025/05/21 15:33:30 by asinsard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "token.h"
#include "list.h"
#include <fcntl.h>

// void	extract_stdin(t_pipex *pipex)
// {
// 	int	len;

// 	while (1)
// 	{
// 		write(1, "> ", 2);
// 		pipex->line = get_next_line(0);
// 		if (!pipex->line)
// 		{
// 			error_message_hd(pipex, pipex->count);
// 			break ;
// 		}
// 		len = ft_strlen(pipex->limiter);
// 		if (ft_strncmp(pipex->line, pipex->limiter, len) == 0)
// 		{
// 			free(pipex->line);
// 			break ;
// 		}
// 		if (write(pipex->fd, pipex->line, ft_strlen(pipex->line)) == -1)
// 		{
// 			free(pipex->line);
// 			ft_error("Problem with the write of 'extract stdin'", errno);
// 		}
// 		free(pipex->line);
// 		pipex->count++;
// 	}
// }

// void	make_here_doc(t_token *node)
// {
// 	int	fd;

// 	fd = open("/tmp/here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 		ft_error("Problem with the here_doc file", NO_F_OR_D);
// 	node->limiter = ft_strjoin(node->av[2], "\n");
// 	if (!node->limiter)
// 	{
// 		close(node->fd);
// 		ft_error("Problem of 'ft_strjoin' of 'make_here_doc'", MEM_ALLOC);
// 	}
// 	extract_stdin(node);
// 	close(node->fd);
// 	free(node->limiter);
// 	node->is_hd = true;
// }

// bool	handle_here_doc(t_token **head)
// {
// 	t_token	*tmp;
// 	bool	flag;

// 	if (!head || !*head)
// 		return (false);
// 	if (!create_hd_name(head))
// 		return (false);
// 	tmp = *head;
// 	flag = false;
// 	while (tmp)
// 	{
// 		if (tmp->token == HD)
// 			flag = create_here_doc(tmp);
// 		tmp = tmp->next;			
// 	}
// 	return (flag);
// }