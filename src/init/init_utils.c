/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:53:47 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/14 16:13:49 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "libft.h"
#include "malloc_error_handlers.h" //pour le malloc error
#include <limits.h>
#include <stdio.h> // le perror
#include <stdlib.h>
#include <unistd.h>

int	find_first_digit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			break ;
		i++;
	}
	return (i);
}

int	get_cwd_init(char *s)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) != NULL)
		s = ft_strdup(buf);
	else
	{
		perror("getcwd");
		return (1);
	}
	if (!s)
		return (-1);
	return (0);
}

int	incremente_shlvl(char *s, char **line)
{
	int		n;
	char	*shlvl_n;

	n = ft_atoi(s + 6);
	if (n < 0)
	{
		*line = ft_strdup("SHLVL=0");
		if (!*line)
			return (-1);
	}
	else
	{
		shlvl_n = ft_itoa(n + 1);
		if (!shlvl_n)
			return (-1);
		*line = ft_strjoin("SHLVL=", shlvl_n);
		if (!*line)
			return (malloc_free_string(shlvl_n));
		free(shlvl_n);
	}
	return (0);
}

int	init_and_incremente_shlvl(char *s, t_var **env)
{
	int		i;
	char	*line;

	i = 7;
	line = NULL;
	i = find_first_digit(s);
	if (!((size_t)i == ft_strlen(s)))
	{
		if (incremente_shlvl(s, &line) == -1)
			return (-1);
	}
	if (!line)
	{
		line = ft_strdup("SHLVL=1");
		if (!line)
			return (-1);
	}
	if (add_back_var(env, line, 3) == -1)
		return (malloc_free_string(line));
	free(line);
	return (0);
}
