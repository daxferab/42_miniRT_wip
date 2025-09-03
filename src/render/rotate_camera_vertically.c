#include "minirt.h"

//TODO special case: orientation is or will be vertical
void	rotate_camera_vertically(t_scene *scene, double direction)
{
	double	angle_before;
	double	angle_after;
	double	horizontal_ratio;

	angle_before = asin(scene->camera->orientation.y);
	angle_after = angle_before + direction;
	horizontal_ratio = cos(angle_after) / cos(angle_before);
	scene->camera->orientation.x
		= scene->camera->orientation.x * horizontal_ratio;
	scene->camera->orientation.y = sin(angle_after);
	scene->camera->orientation.z
		= scene->camera->orientation.z * horizontal_ratio;
	update_camera_axis(scene, scene->camera);
}
