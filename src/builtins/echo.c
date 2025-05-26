
#include "libft.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "echo_utils.h"

int get_options(int *option, char **arg)
{
	int i;

	i = 1;
	while (arg[i] && !ft_strncmp(arg[i], "-n", 2) && is_only_n(arg[i]))
	{
		if (is_a_valid_echo_option(arg[i]))
			*option = 1;
		i++;
	}
	if (*option == 1)
	{
		while (arg[i] && ((!ft_strncmp(arg[i], "-n", 3))))
			i++;
	}
	return (i);
}

int builtin_echo_print(char *s, int option)
{
	char	*tmp;
	int saved_errno;

	if (option == 0)
	{
		tmp = s;
		s = ft_strjoin(s, "\n");
		if (!s)
		{
			saved_errno = errno;
			free(tmp);
			errno = saved_errno;
			return (errno);
		}
		free(tmp);
	}
	if (s)
	{
		ft_putstr_fd(s, 1);
		free(s);
	}
	return (0);
}

int builtin_echo_join_str(char **arg, int i, char **s)
{
	char	*tmp;
	int saved_errno;

	while (arg[i])
	{
		tmp = *s;
		*s = ft_strjoin(*s, arg[i]);
		if (!*s)
		{
			saved_errno = errno;
			free(tmp);
			errno = saved_errno;
			return (errno);
		}
		free(tmp);
		i++;
	}
	return (0);
}

int	builtin_echo(char **arg)
{
	int		option;
	int exit_code;
	int		i;
	char	*s;

	option = 0;
	s = NULL;
	if (!arg[1])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	s = ft_strdup("");
	if (!s)
		return (errno);
	i = get_options(&option, arg);
	exit_code = builtin_echo_join_str(arg, i, &s);
	if (exit_code != 0)
		return (exit_code);
	return (builtin_echo_print(s, option));
}
