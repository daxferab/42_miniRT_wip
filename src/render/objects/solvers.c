#include "minirt.h"

//PLANE

double	solve_plane(t_plane *plane, t_coords origin, t_vector direction)
{
	double	numerator;
	double	denominator;

	numerator = v3_dot_product(
			v3_substract(plane->coords, origin), plane->normal);
	denominator = v3_dot_product(direction, plane->normal);
	if (denominator == 0 && numerator == 0)
		return (0);
	else if (denominator == 0)
		return (-1);
	return (numerator / denominator);
}

//SPHERE

double	solve_sphere(t_sphere *sphere, t_coords origin, t_vector direction)
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

//CYLINDER

static double	solve_disk(t_cylinder *cyl, t_coords origin, t_vector dir, t_plane *plane);
static double	solve_caps(t_cylinder *cyl, t_coords origin, t_vector dir);

double	solve_cylinder(t_cylinder *cyl, t_coords origin, t_vector dir)
{
	t_vector	k;
	t_vector	d;
	t_vector	a;
	double		da;
	double		dd;
	double		disc;
	double		t;

	k = v3_substract(origin, cyl->coords);
	d = v3_substract(dir, v3_scale(cyl->axis, v3_dot_product(dir, cyl->axis)));
	a = v3_substract(k, v3_scale(cyl->axis, v3_dot_product(k, cyl->axis)));
	da = v3_dot_product(d, a);
	dd = v3_dot_product(d, d);
	disc = da * da - dd * (v3_dot_product(a, a) - pow(cyl->radius, 2));

	if (dd == 0) //direction is parallel to axis
		return (-1);
	if (disc < 0)
		return (-1);
	t = (-da - sqrt(disc)) / dd;
	if (t >= 0 && is_in_height(cyl, origin, dir, t))
		return (t);
	if (disc == 0)
		return (-1);
	t = (-da + sqrt(disc)) / dd;
	if (t >= 0 && is_in_height(cyl, origin, dir, t))
		return (t);
	return (-1);
}

static double	solve_disk(t_cylinder *cyl, t_coords origin, t_vector dir, t_plane *plane)
{
	double		distance;
	t_coords	intersection;

	distance = solve_plane(plane, origin, dir);
	if (distance >= 0)
	{
		intersection = ray_at(origin, dir, distance);
		if (v3_magnitude(v3_substract(intersection, plane->coords)) <= cyl->radius)
			return (distance);
	}
	return (-1);
}

static double	solve_caps(t_cylinder *cyl, t_coords origin, t_vector dir)
{
	t_plane		plane;
	double		distance_1;
	double		distance_2;

	plane.normal = cyl->axis;
	plane.coords = ray_at(cyl->coords, cyl->axis, cyl->half_height);
	distance_1 = solve_disk(cyl, origin, dir, &plane);
	plane.coords = ray_at(cyl->coords, cyl->axis, -cyl->half_height);
	distance_2 = solve_disk(cyl, origin, dir, &plane);
	if (distance_1 >= 0 && (distance_1 <= distance_2 || distance_2 < 0))
		return (distance_1);
	if (distance_2 >= 0 && (distance_2 <= distance_1 || distance_1 < 0))
		return (distance_2);
	return (-1);
}
