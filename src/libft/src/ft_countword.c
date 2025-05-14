#include "libft.h"

int	ft_countword(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*str)
	{
		if (!ft_isspace(*str) && i == 0)
		{
			i = 1;
			count++;
		}
		else if (ft_isspace(*str) && i != 0)
			i = 0;
		str++;
	}
	return (count);
}
