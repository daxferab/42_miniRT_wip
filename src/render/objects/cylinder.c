#include "minirt.h"

void	intersect_cylinders(t_scene *scene, t_point *point)
{
	t_cylinder	*cyl;
	double		intersec;
	t_vector	v;

	cyl = scene->cylinder_list;
	while (cyl)
	{
		intersec = solve_cylinder(cyl, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersec, cyl->color))
		{
			v = v3_substract(point->coords, cyl->coords);
			point->normal = v3_normalize(v3_substract(v,
						v3_scale(cyl->axis, v3_dot_product(v, cyl->axis))));
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
		}
		intersec = solve_caps(cyl, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersec, cyl->color))
		{
			point->normal = cyl->axis;
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
		}
		cyl = cyl->next;
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
