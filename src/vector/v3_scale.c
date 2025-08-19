#include "minirt.h"

t_v3	v3_scale(t_v3	v3, double scale)
{
	t_v3	result;

	result.x = v3.x * scale;
	result.y = v3.y * scale;
	result.z = v3.z * scale;
	return (result);
}
