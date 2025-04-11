
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
/* #include <unistd.h> */
#include <stdlib.h>

// Traps ???

int	is_only_numeric_argument(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if ((s[i] > '9' || s[i] < '0') && (s[i] != '-' && s[i] != '+'))
			return (0);
		i++;
	}
	return (1);
}

// a voir
int	builtin_exit(char **arg, t_var **env)
{
	int		n;
	char	*s;

	(void)env;
	s = NULL;
	if (!arg[1])
	{
		free_list(env);
		ft_free(arg);
		// exit avec le dernier code d'erreur  ?
		exit(0);
	}
	if (ft_strlen(arg[1]) > 18)
	{
		s = ft_strjoin(s, "minishell: exit: ", s);
		s = ft_strjoin(s, arg[1], s);
		s = ft_strjoin(s, ": numeric argument required", s);
		ft_putstr_fd(s, 2);
		free(s);
		return (2);
	}
	if (arg[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	// a tester
	if (!is_only_numeric_argument(arg[1]))
	{
		s = ft_strjoin("minishell: exit: ", arg[1], s);
		s = ft_strjoin(s, "numeric argument required\n", s);
		ft_putstr_fd(s, 2);
		free(s);
		free_list(env);
		ft_free(arg);
		exit(2);
	}
	n = ft_atoi(arg[1]);
	if (n > 255)
		n = n % 256;
	free_list(env);
	ft_free(arg);
	exit(n);
	return (0);
}
/* Exit status: */
/*        125    if the env command itself fails */
/**/
/*        126    if COMMAND is found but cannot be invoked */
/**/
/*        127    if COMMAND cannot be found */
/**/
/*        -      the exit status of COMMAND otherwise */
