#include "minirt.h"

t_v3	v3_cross_product(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}
