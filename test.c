
// #include "parser.h"
#include "libft.h"
#include <stdio.h>


char	**split_line(char *line)
{
	
}

int	main(int ac, char **av)
{
	int	i = 0;
	char	**array;
	
	array = ft_split(av[1], ' ');
	while (array[i])
	{
		printf("%d = %s\n", i + 1, array[i]);
		free(array[i]);
		i++;
	}
	free(array);
	return (0);
}