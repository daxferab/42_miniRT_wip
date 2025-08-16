#include "minirt.h"

bool	read_v3_normalized(char **line, struct s_v3 *v3)
{
	if (!read_v3(line, v3))
		return (false);
	if (sqrt(pow(v3->x, 2) + pow(v3->y, 2) + pow(v3->z, 2)) != 1)
		return (false);
	return (true);
}
