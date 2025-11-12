#include "minirt.h"

void	rotate_camera_horizontally(t_scene *scene, double direction)
{
	double	r;
	double	angle;

	r = sqrt(scene->camera->orientation.x * scene->camera->orientation.x
			+ scene->camera->orientation.z * scene->camera->orientation.z);
	angle = atan2(scene->camera->orientation.z, scene->camera->orientation.x)
		+ direction;
	scene->camera->orientation.x = cos(angle) * r;
	scene->camera->orientation.z = sin(angle) * r;
	update_camera_axis(scene, scene->camera);
}
