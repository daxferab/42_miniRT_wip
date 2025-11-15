#include "minirt.h"

void	rotate_camera_vertically(t_scene *scene, double direction)
{
	scene->camera->pitch += direction;
	if (scene->camera->pitch > M_PI / 2)
		scene->camera->pitch = M_PI / 2;
	else if (scene->camera->pitch < -M_PI / 2)
		scene->camera->pitch = -M_PI / 2;
	update_camera_axis(scene, scene->camera);
}
