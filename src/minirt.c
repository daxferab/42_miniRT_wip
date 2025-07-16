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
		printf("Axis %f %f %f\n", cylinder->coords.x, cylinder->coords.y, cylinder->coords.z);
		printf("Diameter %f\n", cylinder->diameter);
		printf("Height %f\n", cylinder->height);
		printf("Color %i %i %i\n", cylinder->color.red, cylinder->color.green, cylinder->color.blue);
		cylinder = cylinder->next;
	}
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
	free_exit(scene, OK);
}
