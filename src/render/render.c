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

void	intersect_planes(t_scene *scene, t_point *point)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = v3_dot_product(v3_substract(plane->coords, scene->camera->coords), plane->normal) / v3_dot_product(point->cam_ray, plane->normal);
		if (intersection > 0 && intersection < point->closest)
		{
			point->closest = intersection;
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
		t_vector k = v3_substract(scene->camera->coords, sphere->coords);
		double disc = pow(v3_dot_product(k, point->cam_ray), 2) - v3_dot_product(k, k) + pow(sphere->diameter / 2, 2);
		double intersection = -1;
		if (disc >= 0)
			intersection = -v3_dot_product(k, point->cam_ray) - sqrt(disc);
		if (intersection > 0 && intersection < point->closest)
		{
			point->closest = intersection;
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
		// BODY

		t_vector k = v3_substract(scene->camera->coords, cylinder->coords);
		t_vector d = point->cam_ray;
		t_vector A = cylinder->axis;
		t_vector D = v3_substract(d, v3_scale(A, v3_dot_product(d, A)));
		t_vector K = v3_substract(k, v3_scale(A, v3_dot_product(k, A)));

		double DK = v3_dot_product(D, K);
		double DD = v3_dot_product(D, D);
		double KK = v3_dot_product(K, K);

		double disc = DK * DK - DD * (KK - pow(cylinder->diameter / 2, 2));
		double intersection = -1;
		if (disc >= 0)
			intersection = (-DK - sqrt(disc)) / DD;

		t_vector intersection_point = v3_add(scene->camera->coords, v3_scale(d, intersection));
		double h = v3_dot_product(v3_substract(intersection_point, cylinder->coords), A);

		if (fabs(h) <= cylinder->height / 2)
		{
			if (intersection > 0 && intersection < point->closest)
			{
				point->closest = intersection;
				change_color(&(point->color), cylinder->color.red, cylinder->color.green, cylinder->color.blue);
			}
		}

		// TOP CAP
		t_coords	center_up = v3_add(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_up = v3_dot_product(v3_substract(center_up, scene->camera->coords), cylinder->axis) / v3_dot_product(point->cam_ray, cylinder->axis);
		t_coords intersection_up = v3_add(scene->camera->coords, v3_scale(point->cam_ray, distance_up));
		if (v3_magnitude(v3_substract(intersection_up, center_up)) <= cylinder->diameter / 2)
		{
			if (distance_up > 0 && distance_up < point->closest)
			{
				point->closest = distance_up;
				change_color(&(point->color), cylinder->color.red, cylinder->color.green, cylinder->color.blue);
			}
		}

		// BOTTOM CAP
		t_coords	center_down = v3_substract(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_down = v3_dot_product(v3_substract(center_down, scene->camera->coords), cylinder->axis) / v3_dot_product(point->cam_ray, cylinder->axis);
		t_coords	intersection_down = v3_add(scene->camera->coords, v3_scale(point->cam_ray, distance_down));
		if (v3_magnitude(v3_substract(intersection_down, center_down)) <= cylinder->diameter / 2)
		{
			if (distance_down > 0 && distance_down < point->closest)
			{
				point->closest = distance_down;
				change_color(&(point->color), cylinder->color.red, cylinder->color.green, cylinder->color.blue);
			}
		}

		cylinder = cylinder->next;
	}
}

bool crash_with_plane(t_scene *scene, t_point point)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = v3_dot_product(v3_substract(plane->coords, point.coords), plane->normal) / v3_dot_product(point.light_ray, plane->normal);
		if (intersection > 0.000001 && intersection < point.light_distance)
			return (true);
		plane = plane->next;
	}
	return (false);
}

//FIXME: Dont illuminate when light is inside sphere and camera isnt
//FIXME: Wrong shadow in sphere
bool crash_with_sphere(t_scene *scene, t_point point)
{
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		t_vector k = v3_substract(point.coords, sphere->coords);
		double disc = pow(v3_dot_product(k, point.light_ray), 2) - v3_dot_product(k, k) + pow(sphere->diameter / 2, 2);
		double intersection = -1;
		if (disc >= 0)
			intersection = -v3_dot_product(k, point.light_ray) - sqrt(disc);
		if (intersection > 0.000001 && intersection < point.light_distance)
			return (true);
		sphere = sphere->next;
	}
	return (false);
}

bool	crash_with_cylinder(t_scene *scene, t_point point)
{
	t_cylinder *cylinder = scene->cylinder_list;
	while (cylinder)
	{
		// BODY

		t_vector k = v3_substract(point.coords, cylinder->coords);
		t_vector d = point.light_ray;
		t_vector A = cylinder->axis;
		t_vector D = v3_substract(d, v3_scale(A, v3_dot_product(d, A)));
		t_vector K = v3_substract(k, v3_scale(A, v3_dot_product(k, A)));

		double DK = v3_dot_product(D, K);
		double DD = v3_dot_product(D, D);
		double KK = v3_dot_product(K, K);

		double disc = DK * DK - DD * (KK - pow(cylinder->diameter / 2, 2));
		double intersection = -1;
		if (disc >= 0)
			intersection = (-DK - sqrt(disc)) / DD;

		t_vector intersection_point = v3_add(point.coords, v3_scale(d, intersection));
		double h = v3_dot_product(v3_substract(intersection_point, cylinder->coords), A);

		if (fabs(h) <= cylinder->height / 2)
		{
			if (intersection > 0.000001 && intersection < point.closest)
				return (true);
		}

		// TOP CAP
		t_coords	center_up = v3_add(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_up = v3_dot_product(v3_substract(center_up, point.coords), cylinder->axis) / v3_dot_product(point.light_ray, cylinder->axis);
		t_coords intersection_up = v3_add(point.coords, v3_scale(point.light_ray, distance_up));
		if (v3_magnitude(v3_substract(intersection_up, center_up)) <= cylinder->diameter / 2)
		{
			if (distance_up > 0.000001 && distance_up < point.closest)
				return (true);
		}

		// BOTTOM CAP
		t_coords	center_down = v3_substract(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_down = v3_dot_product(v3_substract(center_down, point.coords), cylinder->axis) / v3_dot_product(point.light_ray, cylinder->axis);
		t_coords	intersection_down = v3_add(point.coords, v3_scale(point.light_ray, distance_down));
		if (v3_magnitude(v3_substract(intersection_down, center_down)) <= cylinder->diameter / 2)
		{
			if (distance_down > 0.000001 && distance_down < point.closest)
				return (true);
		}

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

bool	has_obstacles(t_scene *scene, t_point point)
{
	return (crash_with_plane(scene, point) || crash_with_sphere(scene, point) || crash_with_cylinder(scene, point));
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
			point.closest = 99999999999999;
			point.cam_ray = get_ray_direction(scene, i, j);
			change_color(&point.color, 0, 0, 0);
			intersect_planes(scene, &point);
			intersect_spheres(scene, &point);
			intersect_cylinders(scene, &point);
			point.coords = v3_add(scene->camera->coords, v3_scale(point.cam_ray, point.closest));
			point.light_ray = v3_normalize(v3_substract(scene->light->coords, point.coords));
			point.light_distance = v3_magnitude(v3_substract(scene->light->coords, point.coords));
			apply_lights(&point, scene->ambient, scene->light, has_obstacles(scene, point));
			mlx_put_pixel(scene->img, i, j, rgb_to_uint(&point.color));
			j++;
		}
		i++;
	}
}
