

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"


//REVOIR BUILD LINE !
// tester en unsetant PWD ...
int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	char	*s;

	s = NULL;
	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		s = build_line(buf, "\n", s);
		ft_putstr_fd(s, 1);
		free(s);
		return (0);
	}
	else
	{
		perror("pwd");
		//  1 ou errno ?
		return (1);
	}
	return (0);
}


