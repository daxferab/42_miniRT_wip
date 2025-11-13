#include "minirt.h"

t_v3	v3_normalize(t_v3 v3)
{
	double	magnitude;

	magnitude = v3_magnitude(v3);
	if (magnitude == 0)
		return (v3);
	return (v3_scale(v3, 1 / magnitude));
}
