#include "minirt.h"

void	move_camera(t_scene *scene)
{
	t_vector	movement;

	if (v3_magnitude(scene->movement) == 0.0)
		return ;
	movement = v3_normalize(v3_add(
				v3_scale(scene->world_up, scene->movement.y), v3_add(
					v3_scale(scene->camera->forward, scene->movement.z),
					v3_scale(scene->camera->rightward, scene->movement.x)
					)));
	scene->camera->coords = v3_add(scene->camera->coords, movement);
}
