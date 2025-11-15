#include "minirt.h"

void	rotate_camera(t_scene *scene)
{
	if (scene->camera->pitch_to_change == 0 && scene->camera->yaw_to_change == 0)
		return ;
	scene->camera->pitch += deg_to_rad(scene->camera->pitch_to_change);
	if (scene->camera->pitch > M_PI / 2)
		scene->camera->pitch = M_PI / 2;
	else if (scene->camera->pitch < -M_PI / 2)
		scene->camera->pitch = -M_PI / 2;
	scene->camera->yaw += deg_to_rad(scene->camera->yaw_to_change);
	if (scene->camera->yaw > M_PI)
		scene->camera->yaw -= M_PI * 2;
	else if (scene->camera->yaw < -M_PI)
		scene->camera->yaw += M_PI * 2;
	update_camera_axis(scene, scene->camera);
}
