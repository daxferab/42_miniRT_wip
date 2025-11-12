#include "minirt.h"

static double	solve_cylinder(t_cylinder *cylinder, t_coords origin, t_vector direction);
static double	solve_disk(t_cylinder *cylinder, t_coords origin, t_vector direction, t_plane *plane);
static double	solve_caps(t_cylinder *cylinder, t_coords origin, t_vector direction);

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
		intersection = solve_cylinder(cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		intersection = solve_caps(cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		cylinder = cylinder->next;
	}
	return (false);
}

static double	solve_cylinder(t_cylinder *cylinder, t_coords origin, t_vector direction)
{
	t_vector k = v3_substract(origin, cylinder->coords);
	t_vector d = direction;
	t_vector A = cylinder->axis;
	t_vector D = v3_substract(d, v3_scale(A, v3_dot_product(d, A)));
	t_vector K = v3_substract(k, v3_scale(A, v3_dot_product(k, A)));

	double DK = v3_dot_product(D, K);
	double DD = v3_dot_product(D, D);
	double KK = v3_dot_product(K, K);

	if (DD == 0) //direction is parallel to axis
		return (-1);
	double disc = DK * DK - DD * (KK - pow(cylinder->radius, 2));
	double t;
	if (disc < 0)
		return (-1);
	t = (-DK - sqrt(disc)) / DD;
	if (t >= 0 && is_in_height(cylinder, origin, direction, t))
		return (t);
	if (disc == 0)
		return (-1);
	t = (-DK + sqrt(disc)) / DD;
	if (t >= 0 && is_in_height(cylinder, origin, direction, t))
		return (t);
	return (-1);
}

static double	solve_disk(t_cylinder *cylinder, t_coords origin, t_vector direction, t_plane *plane)
{
	double		distance;
	t_coords	intersection;

	distance = solve_plane(plane, origin, direction);
	if (distance >= 0)
	{
		intersection = ray_at(origin, direction, distance);
		if (v3_magnitude(v3_substract(intersection, plane->coords)) <= cylinder->radius)
			return (distance);
	}
	return (-1);
}

static double	solve_caps(t_cylinder *cylinder, t_coords origin, t_vector direction)
{
	t_plane		plane;
	double		distance_1;
	double		distance_2;

	plane.normal = cylinder->axis;
	plane.coords = ray_at(cylinder->coords, cylinder->axis, cylinder->half_height);
	distance_1 = solve_disk(cylinder, origin, direction, &plane);
	plane.coords = ray_at(cylinder->coords, cylinder->axis, -cylinder->half_height);
	distance_2 = solve_disk(cylinder, origin, direction, &plane);
	if (distance_1 >= 0 && (distance_1 <= distance_2 || distance_2 < 0))
		return (distance_1);
	if (distance_2 >= 0 && (distance_2 <= distance_1 || distance_1 < 0))
		return (distance_2);
	return (-1);
}
