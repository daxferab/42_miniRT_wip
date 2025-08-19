#include "minirt.h"

t_v3	v3_normalize(t_v3 v3)
{
	return (v3_scale(v3, 1 / v3_magnitude(v3)));
}
