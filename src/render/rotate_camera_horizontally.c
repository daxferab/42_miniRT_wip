#include "minirt.h"

//TODO special case: orientation is or will be vertical
void	rotate_camera_horizontally(t_scene *scene, double direction)
{
	double	r;
	double	angle;

	r = sqrt(pow(scene->camera->orientation.x, 2)
			+ pow(scene->camera->orientation.z, 2));
	angle = atan2(scene->camera->orientation.z, scene->camera->orientation.x)
		+ direction;
	scene->camera->orientation.x = cos(angle) * r;
	scene->camera->orientation.z = sin(angle) * r;
	update_camera_axis(scene, scene->camera);
}
