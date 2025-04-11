#include <stdio.h>
#include <stdlib.h>

int	fibonacci(int n)
{
	int	i;

	i = 0;
	if (n < 2)
		return (n);
	return (fibonacci(n - 1) + fibonacci(n - 2));
}

int	main(int ac, char **av)
{
	int	i;
	int	n;

	i = 0;
	if (ac == 2)
	{
		n = atoi(av[1]);
		while (i < n)
		{
			printf("%d\n", fibonacci(i));
			i++;
		}
	}
	return (0);
}
