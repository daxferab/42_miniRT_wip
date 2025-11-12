#include "minirt.h"

void	move_camera(t_scene *scene)
{
	t_vector	movement;

	if (v3_magnitude(scene->movement) == 0.0)
		return ;
	movement = scene->movement;
	if (v3_magnitude(scene->movement) != 1.0)
		movement = v3_normalize(movement);
	scene->camera->coords = v3_add(scene->camera->coords, movement);
}
