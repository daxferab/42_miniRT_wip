#include "minirt.h"

bool	read_int_maxed(char **line, int *result, int max_range)
{
	if (!ft_isdigit(**line))
		return (false);
	*result = 0;
	while (ft_isdigit(**line))
	{
		*result = *result * 10 + *(*line)++ - '0';
		if (*result > max_range)
			return (false);
	}
	return (true);
}
