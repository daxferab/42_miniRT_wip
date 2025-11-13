#include "minirt.h"

void	update_camera_axis(t_scene *scene, t_camera *cam)
{
	cam->right = v3_normalize(
			v3_cross_product(cam->orientation, scene->world_up));
	cam->up = v3_normalize(v3_cross_product(cam->right, cam->orientation));
	cam->forward = v3_normalize(
			v3_build(cam->orientation.x, 0, cam->orientation.z));
	cam->rightward = v3_cross_product(cam->forward, scene->world_up);
}
