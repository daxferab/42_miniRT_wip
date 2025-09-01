#include "minirt.h"

//TODO special case: orientation is or will be vertical
void	rotate_camera_vertically(t_scene *scene, double direction)
{
	double x = scene->camera->orientation.x;
	double y = scene->camera->orientation.y;
	double z = scene->camera->orientation.z;
	double rad = asin(y);
	double deg = rad_to_deg(rad);
	printf("%+.2f -> %f (%.0fº)\n", y, rad, deg);
	double before = cos(rad);
	deg += direction;
	rad = deg_to_rad(deg);
	y = sin(rad);
	double after = cos(rad);
	x = x / before * after;
	z = z / before * after;
	printf("new %f (%.0fº)   %f -> %f\n\n", rad, deg, before, after);
	scene->camera->orientation.x = x;
	scene->camera->orientation.y = y;
	scene->camera->orientation.z = z;
	update_camera_axis(scene, scene->camera);
}
