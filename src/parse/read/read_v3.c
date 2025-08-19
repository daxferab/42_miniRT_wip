#include "minirt.h"

bool	read_v3(char **line, t_v3 *v3)
{
	if (!read_double(line, &v3->x))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_double(line, &v3->y))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_double(line, &v3->z))
		return (false);
	return (true);
}
