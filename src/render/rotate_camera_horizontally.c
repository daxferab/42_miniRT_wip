#include "minirt.h"

void	rotate_camera_horizontally(t_scene *scene, double direction)
{
	scene->camera->yaw += direction;
	if (scene->camera->yaw > M_PI)
		scene->camera->yaw -= M_PI * 2;
	else if (scene->camera->yaw < -M_PI)
		scene->camera->yaw += M_PI * 2;
	update_camera_axis(scene, scene->camera);
}
