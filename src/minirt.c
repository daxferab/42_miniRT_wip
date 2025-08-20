#include "minirt.h"

void	print_data(t_scene *scene)
{
	if (scene->ambient)
	{
		printf("\n\tAMBIENT\n");
		printf("Color %i %i %i\n", scene->ambient->color.red, scene->ambient->color.green, scene->ambient->color.blue);
		printf("Ratio %f\n", scene->ambient->ratio);
	}
	if (scene->camera)
	{
		printf("\n\tCAMERA\n");
		printf("Coords %f %f %f\n", scene->camera->coords.x, scene->camera->coords.y, scene->camera->coords.z);
		printf("Direction %f %f %f\n", scene->camera->orientation.x, scene->camera->orientation.y, scene->camera->orientation.z);
		printf("FOV %i\n", scene->camera->fov);
	}
	if (scene->light)
	{
		printf("\n\tLIGHT\n");
		printf("Coords %f %f %f\n", scene->light->coords.x, scene->light->coords.y, scene->light->coords.z);
		printf("Ratio %f\n", scene->light->ratio);
	}
	t_plane *plane = scene->plane_list;
	while (plane)
	{
		printf("\n\tPLANE\n");
		printf("Coords %f %f %f\n", plane->coords.x, plane->coords.y, plane->coords.z);
		printf("Normal %f %f %f\n", plane->normal.x, plane->normal.y, plane->normal.z);
		printf("Color %i %i %i\n", plane->color.red, plane->color.green, plane->color.blue);
		plane = plane->next;
	}
	t_sphere *sphere = scene->sphere_list;
	while (sphere)
	{
		printf("\n\tSPHERE\n");
		printf("Coords %f %f %f\n", sphere->coords.x, sphere->coords.y, sphere->coords.z);
		printf("Diameter %f\n", sphere->diameter);
		printf("Color %i %i %i\n", sphere->color.red, sphere->color.green, sphere->color.blue);
		sphere = sphere->next;
	}
	t_cylinder *cylinder = scene->cylinder_list;
	while (cylinder)
	{
		printf("\n\tCYLINDER\n");
		printf("Coords %f %f %f\n", cylinder->coords.x, cylinder->coords.y, cylinder->coords.z);
		printf("Axis %f %f %f\n", cylinder->axis.x, cylinder->axis.y, cylinder->axis.z);
		printf("Diameter %f\n", cylinder->diameter);
		printf("Height %f\n", cylinder->height);
		printf("Color %i %i %i\n", cylinder->color.red, cylinder->color.green, cylinder->color.blue);
		cylinder = cylinder->next;
	}
}

void	draw(t_scene *scene)
{
	double width = 512;
	double height = 512;
	double fov = 120;

	double scale = tan(fov/2 * 3.14159265358979323846 / 180);
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
			t_v3 right = v3_normalize(v3_cross_product(forward, (t_v3){0,0,1}));
			t_v3 up = v3_normalize(v3_cross_product(forward, right));
			t_v3 rd = v3_normalize(v3_add(v3_add(v3_scale(right,iter_x), v3_scale(up,iter_y)), forward));
			float closest = 99999999999999999;
			uint32_t color;
			t_plane *iter = scene->plane_list;
			while (iter)
			{
				float intersection = v3_dot_product(v3_substract(iter->coords, scene->camera->coords), iter->normal) / v3_dot_product(rd, iter->normal);
				if (intersection > 0 && intersection < closest)
				{
					closest = intersection;
					color = iter->color.red * pow(256, 3) + iter->color.green * pow(256, 2) + iter->color.blue * pow(256, 1) + 255;
				}
				iter = iter->next;
			}
			iter_x += step;
			j++;
			mlx_put_pixel(scene->img, i, j, color);
		}
		iter_y -= step;
		i++;
	}
}

void	loop_hook(void *param)
{
	t_scene	*scene;

	scene = param;
	draw(scene);
	mlx_image_to_window(scene->mlx, scene->img, 0, 0);//protect
}

int	main(int argc, char **argv)
{
	t_scene	*scene;

	scene = ft_calloc(1, sizeof(t_scene));
	if (argc != 2)
		free_exit(scene, ERR_ARGUMENTS);
	if (!scene)
		free_exit(scene, ERR_ALLOC);
	parse_file(scene, argv[1]);
	print_data(scene);
	scene->mlx = mlx_init(512, 512, "MiniRT", false);
	if (!scene->mlx)
		free_exit(scene, OK);
	scene->img = mlx_new_image(scene->mlx, 512, 512);//protect
	mlx_close_hook(scene->mlx, &close_hook, scene);
	mlx_key_hook(scene->mlx, &key_hook, scene);
	mlx_loop_hook(scene->mlx, &loop_hook, scene);
	mlx_loop(scene->mlx);
	return (0);
}
