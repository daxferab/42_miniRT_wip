#include "minirt.h"

void	update_camera_axis(t_scene *scene, t_camera *cam)
{
	t_v3	direction;

	direction.x = cos(scene->camera->pitch) * cos(scene->camera->yaw);
	direction.y = sin(scene->camera->pitch);
	direction.z = cos(scene->camera->pitch) * sin(scene->camera->yaw);
	cam->orientation = v3_build(direction.x, direction.y, direction.z);
	cam->forward = v3_normalize(v3_build(direction.x, 0, direction.z));
	cam->right = v3_normalize(v3_cross_product(cam->forward, scene->world_up));
	if (fabs(cam->orientation.y) != 1)
		cam->up = v3_normalize(v3_cross_product(cam->right, cam->orientation));
	else
		cam->up = v3_scale(cam->forward, -cam->orientation.y);
}
