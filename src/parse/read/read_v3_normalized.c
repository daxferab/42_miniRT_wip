#include "minirt.h"

bool	read_v3_normalized(char **line, t_v3 *v3)
{
	if (!read_v3(line, v3))
		return (false);
	if (v3_magnitude(*v3) != 1.0)
		return (false);
	return (true);
}
