#include "minirt.h"

t_v3	v3_add(t_v3 a, t_v3 b)
{
	return (v3_build(
			a.x + b.x,
			a.y + b.y,
			a.z + b.z
		));
}
