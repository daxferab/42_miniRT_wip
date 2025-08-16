#include "minirt.h"

bool	read_double_ratio(char **line, double *result)
{
	if (!read_double(line, result))
		return (false);
	if (*result < 0.0 || *result > 1.0)
		return (false);
	return (true);
}
