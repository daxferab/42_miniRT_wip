#include "minirt.h"

void	intersect_cylinders(t_scene *scene, t_point *point)
{
	t_cylinder	*cylinder;
	double		intersection;
	t_vector	v;

	cylinder = scene->cylinder_list;
	while (cylinder)
	{
		intersection = solve_cylinder(cylinder, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			v = v3_substract(point->coords, cylinder->coords);
			point->normal = v3_normalize(v3_substract(v, v3_scale(cylinder->axis, v3_dot_product(v, cylinder->axis))));
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
			change_color(&(point->color), cylinder->color.red, cylinder->color.green, cylinder->color.blue);
		}
		intersection = solve_caps(cylinder, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			point->normal = cylinder->axis;
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
			change_color(&(point->color), cylinder->color.red, cylinder->color.green, cylinder->color.blue);
		}
		cylinder = cylinder->next;
	}
}

bool	crash_with_cylinders(t_scene *scene, t_point *point)
{
	t_cylinder	*cylinder;
	double		intersection;

	cylinder = scene->cylinder_list;
	while (cylinder)
	{
		intersection = solve_cylinder(
				cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		intersection = solve_caps(
				cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		cylinder = cylinder->next;
	}
	return (false);
}
