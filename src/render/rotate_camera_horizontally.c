#include "minirt.h"

void	rotate_camera_horizontally(t_scene *scene, double direction)
{
	double x = scene->camera->orientation.x;
	double z = scene->camera->orientation.z;
	double r = sqrt(x * x + z * z);
	double rad = atan2(z, x);
	double deg = rad_to_deg(rad);
	printf("%+.2f %+.2f -> %f (%.0fº)\n", x, z, rad, deg);
	deg += direction;
	rad = deg_to_rad(deg);
	x = cos(rad) * r;
	z = sin(rad) * r;
	printf("%+.2f %+.2f -> %f (%.0fº)\n\n", x, z, rad, deg);
	scene->camera->orientation.x = x;
	scene->camera->orientation.z = z;
	update_camera_axis(scene, scene->camera);
}
