#include "minirt.h"

t_vector	get_ray_direction(t_scene *scene, int i, int j)
{
	double		x;
	double		y;
	t_vector	rd;

	x = (2 * i - WIDTH + 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x), v3_scale(scene->camera->up, y)), scene->camera->orientation));
	return (rd);
}

t_coords	ray_at(t_coords origin, t_vector direction, double distance)
{
	return (v3_add(origin, v3_scale(direction, distance)));
}

void	change_color(t_color *color, int red, int green, int blue)
{
	color->red = red;
	color->green = green;
	color->blue = blue;
}

bool	is_in_height(t_cylinder *cylinder, t_coords origin, t_vector direction, double distance)
{
	t_vector	intersection;
	double		height;

	intersection = v3_add(origin, v3_scale(direction, distance));
	height = v3_dot_product(v3_substract(intersection, cylinder->coords), cylinder->axis);
	return (fabs(height) <= cylinder->half_height);
}

bool	is_closer(t_scene *scene, t_point *point, double intersection)
{
	if (intersection <= 0 || (intersection >= point->closest && point->closest >= 0))
		return (false);
	point->closest = intersection;
	point->coords = ray_at(scene->camera->coords, point->cam_ray, intersection);
	return (true);
}
