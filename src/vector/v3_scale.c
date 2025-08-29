#include "minirt.h"

t_v3	v3_scale(t_v3 v3, double scale)
{
	return (v3_build(
			v3.x * scale,
			v3.y * scale,
			v3.z * scale
		));
}
