#include "minirt.h"

void	render(t_scene *scene)
{
	double scale = tan(DEG_TO_RAD(scene->camera->fov / 2));
	double step = 2.0 / WIDTH * scale;
	double start_x = -scale + scale / WIDTH;
	double start_y = scale / WIDTH * HEIGHT - scale / WIDTH;
	double iter_y = start_y;

	double i = 0;
	while (i < WIDTH)
	{
		double iter_x = start_x;
		double j = 0;
		while (j < HEIGHT)
		{
			t_v3 rd = v3_normalize(v3_add(v3_add(v3_scale(scene->camera->right, iter_x), v3_scale(scene->camera->up, iter_y)), scene->camera->orientation));
			float closest = 99999999999999999;
			uint32_t color;
			t_plane *plane = scene->plane_list;
			while (plane)
			{
				float intersection = v3_dot_product(v3_substract(plane->coords, scene->camera->coords), plane->normal) / v3_dot_product(rd, plane->normal);
				if (intersection > 0 && intersection < closest)
				{
					closest = intersection;
					color = plane->color.red * pow(256, 3) + plane->color.green * pow(256, 2) + plane->color.blue * pow(256, 1) + 255;
				}
				plane = plane->next;
			}
			iter_x += step;
			j++;
			mlx_put_pixel(scene->img, i, j, color);
		}
		iter_y -= step;
		i++;
	}
}
