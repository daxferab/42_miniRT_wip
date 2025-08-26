#include "minirt.h"

//TODO special case: orientation is up
void	update_camera_axis(t_camera *cam)
{
	cam->right = v3_normalize(v3_cross_product(cam->orientation, (t_v3){0, 1, 0}));
	cam->up = v3_normalize(v3_cross_product(cam->orientation, cam->right));
	cam->forward = v3_normalize((t_v3){cam->orientation.x, 0, cam->orientation.z});
	cam->rightward = v3_cross_product((t_v3){0, 1, 0}, cam->forward);
}
