#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

int	main(void)
{
	char	**str;

	str = malloc(sizeof(char *) * 2);
	str[0] = ft_strdup("Salut");
	str[1] = NULL;
	printf("%s\n", str[0]);
	free_tab(str);
	return (0);
}