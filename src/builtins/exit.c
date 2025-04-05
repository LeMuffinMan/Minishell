
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
/* #include <unistd.h> */
#include <stdlib.h>

int is_only_numeric_argument(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] > '9' || s[i] < '0')
			return (0);
		i++;
	}
	return (1);
}



static void join_strs(char *line, va_list strs)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	str = va_arg(strs, char *);
	while (str)
	{
		j = 0;
		while (str[j])
			line[i++] = str[j++];
		str = va_arg(strs, char *);
	}
	line[i] = '\0';
}


char *build_line(char *s, ...)
{
	va_list	strs;
	int		line_len;
	char	*line;
	char	*str;

	line_len = 0;
	va_start(strs, s);
	str = va_arg(strs, char *);
	while (str)
	{
		line_len += ft_strlen(str);
		str = va_arg(strs, char *);
	}
	va_end(strs);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	va_start(strs, s);
	join_strs(line, strs);
	va_end(strs);

	return (line);
}

// a voir
int	builtin_exit(char **arg, t_env **env)
{
	int	n;
	char *s;

	s = NULL;
	if (!arg[1])
	{
		free_list(env); //double free ?
		exit(0);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	//a tester
	if (!is_only_numeric_argument(arg[1]))
	{
		s = build_line(NULL, "minishell: exit: ", arg[1], ": numeric argument required\n", NULL);
		ft_putstr_fd(s, 1);
		free(s);
		/* free_list(env); */ // segfault ?
		exit (2);
	}
	//Si on a autre chose que des digits : 
	//message d'erreur + retour d'erreur 2
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	exit(n);
	return (0);
}
