#include "minirt.h"

void	render(t_scene *scene)
{
	double width = 512;
	double height = 512;
	double fov = 120;

	double scale = tan(fov / 2 * 3.14159265358979323846 / 180);
	double step = 2.0 / width * scale;
	double start_x = -scale + scale / width;
	double start_y = scale / width * height - scale / width;
	double iter_y = start_y;

	double i = 0;
	while (i < width)
	{
		double iter_x = start_x;
		double j = 0;
		while (j < height)
		{
			t_v3 forward = v3_normalize(scene->camera->orientation);
			t_v3 right = v3_normalize(v3_cross_product(forward, (t_v3){0, 0, 1}));
			t_v3 up = v3_normalize(v3_cross_product(forward, right));
			t_v3 rd = v3_normalize(v3_add(v3_add(v3_scale(right, iter_x), v3_scale(up, iter_y)), forward));
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
