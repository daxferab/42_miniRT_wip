#include "minirt.h"

t_vector	get_ray_direction(t_scene *scene, int i, int j)
{
	double x = (2 * i - WIDTH + 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	double y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	t_vector rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x), v3_scale(scene->camera->up, y)), scene->camera->orientation));
	return (rd);
}

void	change_color(t_color *color, int red, int green, int blue)
{
	color->red = red;
	color->green = green;
	color->blue = blue;
}

t_coords	ray_at(t_coords origin, t_vector direction, double distance)
{
	return (v3_add(origin, v3_scale(direction, distance)));
}

double	solve_plane(t_plane *plane, t_coords origin, t_vector direction)
{
	double	numerator;
	double	denominator;

	numerator = v3_dot_product(v3_substract(plane->coords, origin), plane->normal);
	denominator = v3_dot_product(direction, plane->normal);
	if (denominator == 0 && numerator == 0)
		return (0);
	else if (denominator == 0)
		return (-1);
	return (numerator / denominator);
}

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

bool	is_in_height(t_cylinder *cylinder, t_coords origin, t_vector direction, double distance)
{
	t_vector	intersection;
	double		height;

	intersection = v3_add(origin, v3_scale(direction, distance));
	height = v3_dot_product(v3_substract(intersection, cylinder->coords), cylinder->axis);
	return (fabs(height) <= cylinder->half_height);
}

double	solve_cylinder(t_cylinder *cylinder, t_coords origin, t_vector direction)
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
	if (t >= 0 && is_in_height(cylinder,origin, direction, t))
		return (t);
	if (disc == 0)
		return (-1);
	t = (-DK + sqrt(disc)) / DD;
	if (t >= 0 && is_in_height(cylinder,origin, direction, t))
		return (t);
	return (-1);
}

double	solve_disk(t_cylinder *cylinder, t_coords origin, t_vector direction, t_plane *plane)
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

double	solve_caps(t_cylinder *cylinder, t_coords origin, t_vector direction)
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
	return(-1);
}

bool	is_closer(t_scene *scene, t_point *point, double intersection)
{
	if (intersection <= 0 || (intersection >= point->closest && point->closest >= 0))
		return (false);
	point->closest = intersection;
	point->coords = ray_at(scene->camera->coords, point->cam_ray, intersection);
	return (true);
}

void	intersect_planes(t_scene *scene, t_point *point)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = solve_plane(plane, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			point->normal = plane->normal;
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
			change_color(&(point->color), plane->color.red, plane->color.green, plane->color.blue);
		}
		plane = plane->next;
	}
}

void	intersect_spheres(t_scene *scene, t_point *point)
{
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		double intersection = solve_sphere(sphere, scene->camera->coords, point->cam_ray);
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

void	intersect_cylinders(t_scene *scene, t_point *point)
{
	t_cylinder *cylinder = scene->cylinder_list;
	while (cylinder)
	{
		double intersection = solve_cylinder(cylinder, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			t_vector V = v3_substract(point->coords, cylinder->coords);
			point->normal = v3_normalize(v3_substract(V, v3_scale(cylinder->axis, v3_dot_product(V, cylinder->axis))));
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

bool crash_with_planes(t_scene *scene, t_point *point)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = solve_plane(plane, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		plane = plane->next;
	}
	return (false);
}

bool crash_with_spheres(t_scene *scene, t_point *point)
{
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		double intersection = solve_sphere(sphere, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		sphere = sphere->next;
	}
	return (false);
}

bool	crash_with_cylinders(t_scene *scene, t_point *point)
{
	t_cylinder *cylinder = scene->cylinder_list;
	while (cylinder)
	{
		double intersection = solve_cylinder(cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		intersection = solve_caps(cylinder, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		cylinder = cylinder->next;
	}
	return (false);
}

void	apply_ambient(t_ambient *ambient, t_color *color)
{
	color->red *= ambient->color.red * ambient->ratio / 255;
	color->green *= ambient->color.green * ambient->ratio / 255;
	color->blue *= ambient->color.blue * ambient->ratio / 255;
}

void	apply_light(t_light *light, double distance, t_color color, t_color *final)
{
	double attenuation = 1 / (1 + 0.1 * distance + 0.01 * pow(distance, 2));
	double intensity = light->ratio * attenuation;
	final->red += color.red * intensity;
	final->green += color.green * intensity;
	final->blue += color.blue * intensity;
}

void	clamp_values(t_color *color)
{
	if (color->red > 255)
		color->red = 255;
	if (color->green > 255)
		color->green = 255;
	if (color->blue > 255)
		color->blue = 255;
}

void	apply_lights(t_point *point, t_ambient *ambient, t_light *light, bool in_shadow)
{
	t_color final;
	t_color add_light;
	change_color(&final, point->color.red, point->color.green, point->color.blue);
	apply_ambient(ambient, &final);
	if (!in_shadow)
	{
		change_color(&add_light, point->color.red, point->color.green, point->color.blue);
		apply_light(light, point->light_distance, add_light, &final);
	}
	clamp_values(&final);
	point->color = final;
}

bool	light_is_behind(t_point *point)
{
	return (v3_dot_product(point->normal, point->light_ray) < 0);
}

bool	has_obstacles(t_scene *scene, t_point *point)
{
	return (light_is_behind(point) || crash_with_planes(scene, point) || crash_with_spheres(scene, point) || crash_with_cylinders(scene, point));
}

void	render(t_scene *scene)
{
	double	i;
	double	j;
	t_point	point;

	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			point.closest = -1;
			point.cam_ray = get_ray_direction(scene, i, j);
			change_color(&point.color, 0, 0, 0);
			intersect_planes(scene, &point);
			intersect_spheres(scene, &point);
			intersect_cylinders(scene, &point);
			if (point.closest >= 0)
			{
				point.light_ray = v3_normalize(v3_substract(scene->light->coords, point.coords));
				point.light_distance = v3_magnitude(v3_substract(scene->light->coords, point.coords));
				point.coords_adjusted = ray_at(point.coords, point.normal, EPSILON);
				apply_lights(&point, scene->ambient, scene->light, has_obstacles(scene, &point));
			}
			mlx_put_pixel(scene->img, i, j, rgb_to_uint(&point.color));
			j++;
		}
		i++;
	}
}
