#include "minirt.h"

t_v3	v3_cross_product(t_v3 a, t_v3 b)
{
	return (v3_build(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		));
}
