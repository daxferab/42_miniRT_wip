#include "minirt.h"

t_vector	get_ray_direction(t_scene *scene, int i, int j)
{
	double x = (2 * i - WIDTH + 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	double y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	t_vector rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x), v3_scale(scene->camera->up, y)), scene->camera->orientation));
	return (rd);
}

void	intersect_planes(t_scene *scene, t_vector rd, double *closest, uint32_t *color)
{
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		double intersection = v3_dot_product(v3_substract(plane->coords, scene->camera->coords), plane->normal) / v3_dot_product(rd, plane->normal);
		if (intersection > 0 && intersection < *closest)
		{
			*closest = intersection;
			*color = plane->color.red * pow(256, 3) + plane->color.green * pow(256, 2) + plane->color.blue * pow(256, 1) + 255;
		}
		plane = plane->next;
	}
}

void	intersect_spheres(t_scene *scene, t_vector rd, double *closest, uint32_t *color)
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
			*color = sphere->color.red * pow(256, 3) + sphere->color.green * pow(256, 2) + sphere->color.blue * pow(256, 1) + 128;
		}
		sphere = sphere->next;
	}
}
void	intersect_cylinders(t_scene *scene, t_vector rd, double *closest, uint32_t *color)
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
				*color = cylinder->color.red * pow(256, 3) + cylinder->color.green * pow(256, 2) + cylinder->color.blue * pow(256, 1) + 255;
			}
		}

		// BOTTOM CAP
		t_coords	center_down = v3_substract(cylinder->coords, v3_scale(cylinder->axis, cylinder->height / 2));
		double	distance_down = v3_dot_product(v3_substract(center_down, scene->camera->coords), cylinder->axis) / v3_dot_product(rd, cylinder->axis);
		t_coords	intersection_down = v3_add(scene->camera->coords, v3_scale(rd, distance_down));;
		if (v3_magnitude(v3_substract(intersection_down, center_down)) <= cylinder->diameter / 2)
		{
			if (distance_down > 0 && distance_down < *closest)
			{
				*closest = distance_down;
				*color = cylinder->color.red * pow(256, 3) + cylinder->color.green * pow(256, 2) + cylinder->color.blue * pow(256, 1) + 255;
			}
		}
		
		// BODY
		cylinder = cylinder->next;
	}
}

void	render(t_scene *scene)
{
	double i = 0;
	while (i < WIDTH)
	{
		double j = 0;
		while (j < HEIGHT)
		{
			t_vector cam_rd = get_ray_direction(scene, i, j);
			double closest = 99999999999999;
			uint32_t color = 255;
			intersect_planes(scene, cam_rd, &closest, &color);
			intersect_spheres(scene, cam_rd, &closest, &color);
			intersect_cylinders(scene, cam_rd, &closest, &color);
			//TODO: Apply ambient light
			t_vector light_rd = v3_substract(scene->light->coords, v3_add(scene->camera->coords, v3_scale(cam_rd, closest)));
			mlx_put_pixel(scene->img, i, j, color);
			j++;
		}
		i++;
	}
}
