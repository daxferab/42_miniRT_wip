#include "minirt.h"

double	v3_magnitude(t_v3 v3)
{
	return (sqrt(pow(v3.x, 2) + pow(v3.y, 2) + pow(v3.z, 2)));
}
