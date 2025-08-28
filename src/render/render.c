#include "minirt.h"

double	deg_to_rad(double x)
{
	return (x * M_PI / 180);
}

t_v3	get_ray_direction(t_scene *scene, int i, int j)
{
	double x = (2 * i - WIDTH + 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	double y = (-2 * j + HEIGHT - 1) * tan(deg_to_rad(scene->camera->fov * 0.5)) / WIDTH;
	t_v3 rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, x), v3_scale(scene->camera->up, y)), scene->camera->orientation));
	return (rd);
}

void	intersect_planes(t_scene *scene, t_v3 rd, double *closest, uint32_t *color)
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

void	render(t_scene *scene)
{
	double i = 0;
	while (i < WIDTH)
	{
		double j = 0;
		while (j < HEIGHT)
		{
			t_v3 rd = get_ray_direction(scene, i, j);
			double closest = 99999999999999999;
			uint32_t color;
			intersect_planes(scene, rd, &closest, &color);
			mlx_put_pixel(scene->img, i, j, color);
			j++;
		}
		i++;
	}
}
