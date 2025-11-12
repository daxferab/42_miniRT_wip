#include "minirt.h"

//TODO special case: orientation is vertical
void	update_camera_axis(t_scene *scene, t_camera *cam)
{
	if (fabs(cam->orientation.y) != 1)
	{
		cam->right = v3_normalize(v3_cross_product(cam->orientation, scene->world_up));
		cam->up = v3_normalize(v3_cross_product(cam->right, cam->orientation));
		cam->forward = v3_normalize(v3_build(cam->orientation.x, 0, cam->orientation.z));
		cam->rightward = v3_cross_product(cam->forward, scene->world_up);
		return ;
	}
	cam->right = scene->world_east;
	cam->up = v3_scale(scene->world_south, cam->orientation.y);
	cam->forward = v3_scale(scene->world_south, -1);
	cam->rightward = scene->world_east;
}
