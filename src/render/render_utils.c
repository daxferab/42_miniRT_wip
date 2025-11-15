#include "minirt.h"

t_vector	get_ray_direction(t_scene *scene, int i, int j)
{
	double		x;
	double		y;
	t_vector	rd;
	int			fov;

	fov = scene->camera->fov;
	x = (2 * i - WIDTH + 1) * tan(deg_to_rad(fov * 0.5)) / WIDTH;
	y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(fov * 0.5)) / WIDTH;
	rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x),
					v3_scale(scene->camera->up, y)),
				scene->camera->orientation));
	return (rd);
}

t_coords	ray_at(t_coords origin, t_vector direction, double distance)
{
	return (v3_add(origin, v3_scale(direction, distance)));
}

bool	is_in_height(t_cylinder *cyl, t_coords origin, t_vector dir, double dis)
{
	t_vector	intersection;
	double		height;

	intersection = v3_add(origin, v3_scale(dir, dis));
	height = v3_dot_product(v3_substract(intersection, cyl->coords), cyl->axis);
	return (fabs(height) <= cyl->half_height);
}

bool	is_closer(t_scene *scene,
	t_point *point, double intersec, t_color color)
{
	if (intersec <= 0 || (intersec >= point->closest && point->closest >= 0))
		return (false);
	point->closest = intersec;
	point->coords = ray_at(scene->camera->coords, point->cam_ray, intersec);
	point->color = color;
	return (true);
}
