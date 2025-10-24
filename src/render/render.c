#include "minirt.h"

void	change_color(t_color *color, int red, int green, int blue);

t_vector	get_ray_direction(t_scene *scene, int i, int j)
{
	double x = (2 * i - WIDTH + 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	double y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	t_vector rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x), v3_scale(scene->camera->up, y)), scene->camera->orientation));
	return (rd);
}

void	intersect_planes(t_scene *scene, t_vector rd, double *closest, t_color *color)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = v3_dot_product(v3_substract(plane->coords, scene->camera->coords), plane->normal) / v3_dot_product(rd, plane->normal);
		if (intersection > 0 && intersection < *closest)
		{
			*closest = intersection;
			change_color(color, plane->color.red, plane->color.green, plane->color.blue);
		}
		plane = plane->next;
	}
}

void	intersect_spheres(t_scene *scene, t_vector rd, double *closest, t_color *color)
{
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		t_vector k = v3_substract(scene->camera->coords, sphere->coords);
		double disc = pow(v3_dot_product(k, rd), 2) - v3_dot_product(k, k) + pow(sphere->diameter, 2);
		double intersection = -1;
		if (disc >= 0)
			intersection = -v3_dot_product(k, rd) - sqrt(disc);
		if (intersection > 0 && intersection < *closest)
		{
			*closest = intersection;
			change_color(color, sphere->color.red, sphere->color.green, sphere->color.blue);
		}
		sphere = sphere->next;
	}
}

void	intersect_cylinders(t_scene *scene, t_vector rd, double *closest, t_color *color)
{
	t_cylinder *cylinder = scene->cylinder_list;
	while (cylinder)
	{
		// TOP CAP
		t_coords	center_up = v3_add(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_up = v3_dot_product(v3_substract(center_up, scene->camera->coords), cylinder->axis) / v3_dot_product(rd, cylinder->axis);
		t_coords intersection_up = v3_add(scene->camera->coords, v3_scale(rd, distance_up));
		if (v3_magnitude(v3_substract(intersection_up, center_up)) <= cylinder->diameter / 2)
		{
			if (distance_up > 0 && distance_up < *closest)
			{
				*closest = distance_up;
				change_color(color, cylinder->color.red, cylinder->color.green, cylinder->color.blue);
			}
		}

		// BOTTOM CAP
		t_coords	center_down = v3_substract(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_down = v3_dot_product(v3_substract(center_down, scene->camera->coords), cylinder->axis) / v3_dot_product(rd, cylinder->axis);
		t_coords	intersection_down = v3_add(scene->camera->coords, v3_scale(rd, distance_down));
		if (v3_magnitude(v3_substract(intersection_down, center_down)) <= cylinder->diameter / 2)
		{
			if (distance_down > 0 && distance_down < *closest)
			{
				*closest = distance_down;
				change_color(color, cylinder->color.red, cylinder->color.green, cylinder->color.blue);
			}
		}
		
		// BODY
		cylinder = cylinder->next;
	}
}

bool crash_with_plane(t_scene *scene, t_coords origin, t_vector rd, double distance)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = v3_dot_product(v3_substract(plane->coords, origin), plane->normal) / v3_dot_product(rd, plane->normal);
		if (intersection > 0.000001 && intersection < distance)
			return (true);
		plane = plane->next;
	}
	return (false);
}

bool crash_with_sphere(t_scene *scene, t_coords origin, t_vector rd, double distance)
{
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		t_vector k = v3_substract(origin, sphere->coords);
		double disc = pow(v3_dot_product(k, rd), 2) - v3_dot_product(k, k) + pow(sphere->diameter, 2);
		double intersection = -1;
		if (disc >= 0)
			intersection = -v3_dot_product(k, rd) - sqrt(disc);
		if (intersection > 0.000001 && intersection < distance)
			return (true);
		sphere = sphere->next;
	}
	return (false);
}

void	apply_ambient(t_ambient *ambient, t_color *color)
{
	color->red = color->red * ambient->color.red * (ambient->ratio) / 255;
	color->green = color->green * ambient->color.green * (ambient->ratio) / 255;
	color->blue = color->blue * ambient->color.blue * (ambient->ratio) / 255;
}

void	apply_light(t_light *light, t_color *color)
{
	color->red = color->red * (light->ratio) / 255;
	color->green = color->green * (light->ratio) / 255;
	color->blue = color->blue * (light->ratio) / 255;
}

void	apply_lights(t_color *color, t_ambient *ambient, t_light *light, double distance, bool in_shadow)
{
	(void)distance; //TODO: apply distance
	t_color with_ambient;
	t_color	with_light;
	apply_ambient(ambient, &with_ambient);
	apply_light(light, &with_light);
	if (in_shadow)
		change_color(&with_light, 0, 0, 0);
	color->red = with_ambient.red + with_light.red;
	color->green = with_ambient.green + with_light.green;
	color->blue = with_ambient.blue + with_light.blue;
}

bool	has_obstacles(t_scene *scene,  t_coords origin, t_vector rd, double distance)
{
	if (crash_with_plane(scene, origin, rd, distance) || crash_with_sphere(scene, origin, rd, distance)) //TODO: Crash with cylinder
		return (true);
	return (false);
}

void	change_color(t_color *color, int red, int green, int blue)
{
	color->red = red;
	color->green = green;
	color->blue = blue;
}

void	render(t_scene *scene)
{
	double i = 0;
	while (i < WIDTH)
	{
		double j = 0;
		while (j < HEIGHT)
		{
			t_vector	cam_rd = get_ray_direction(scene, i, j);
			double		closest = 99999999999999;
			t_color		color;
			change_color(&color, 0, 0, 0);
			intersect_planes(scene, cam_rd, &closest, &color);
			intersect_spheres(scene, cam_rd, &closest, &color);
			intersect_cylinders(scene, cam_rd, &closest, &color);
			t_coords	ray_origin = v3_add(scene->camera->coords, v3_scale(cam_rd, closest));
			t_vector	light_rd = v3_normalize(v3_substract(scene->light->coords, ray_origin));
			double		dist_to_light = v3_magnitude(v3_substract(scene->light->coords, ray_origin));
			apply_lights(&color, scene->ambient, scene->light, dist_to_light, has_obstacles(scene, ray_origin, light_rd, dist_to_light));
			mlx_put_pixel(scene->img, i, j, rgb_to_uint(&color));
			j++;
		}
		i++;
	}
}
