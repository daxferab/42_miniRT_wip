#include "minirt.h"

static double	solve_sphere(t_sphere *sphere, t_coords origin, t_vector direction);

void	intersect_spheres(t_scene *scene, t_point *point)
{
	t_sphere	*sphere;
	double		intersection;

	sphere = scene->sphere_list;
	while (sphere)
	{
		intersection = solve_sphere(sphere, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			point->normal = v3_normalize(v3_substract(point->coords, sphere->coords));
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
			change_color(&(point->color), sphere->color.red, sphere->color.green, sphere->color.blue);
		}
		sphere = sphere->next;
	}
}

bool	crash_with_spheres(t_scene *scene, t_point *point)
{
	t_sphere	*sphere;
	double		intersection;

	sphere = scene->sphere_list;
	while (sphere)
	{
		intersection = solve_sphere(sphere, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		sphere = sphere->next;
	}
	return (false);
}

static double	solve_sphere(t_sphere *sphere, t_coords origin, t_vector direction)
{
	t_vector	k;
	double		dk;
	double		disc;

	k = v3_substract(origin, sphere->coords);
	dk = v3_dot_product(k, direction);
	disc = pow(dk, 2) - v3_dot_product(k, k) + pow(sphere->radius, 2);
	if (disc < 0)
		return (-1);
	else if (disc == 0)
		return (-dk);
	else if (-dk - sqrt(disc) >= 0)
		return (-dk - sqrt(disc));
	return (-dk + sqrt(disc));
}
