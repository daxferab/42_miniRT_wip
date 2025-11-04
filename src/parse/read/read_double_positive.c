#include "minirt.h"

bool	read_double_positive_halfed(char **line, double *result)
{
	if (!read_double(line, result))
		return (false);
	if (*result <= 0.0)
		return (false);
	*result /= 2;
	return (true);
}
