#include "minirt.h"

static void	free_plane_list(t_plane *plane_list);
static void	free_sphere_list(t_sphere *sphere_list);
static void	free_cylinder_list(t_cylinder *cylinder_list);

void	free_exit(t_scene *scene, t_error error)
{
	if (error != OK)
		ft_printf("Error\n%i\n", error);
	free(scene->ambient);
	free(scene->camera);
	free(scene->light);
	free_plane_list(scene->plane_list);
	free_sphere_list(scene->sphere_list);
	free_cylinder_list(scene->cylinder_list);
	free(scene);
	exit(error);
}

static void	free_plane_list(t_plane *plane_list)
{
	t_plane	*plane;

	while (plane_list)
	{
		plane = plane_list;
		plane_list = plane_list->next;
		free(plane);
	}
}

static void	free_sphere_list(t_sphere *sphere_list)
{
	t_sphere	*sphere;

	while (sphere_list)
	{
		sphere = sphere_list;
		sphere_list = sphere_list->next;
		free(sphere);
	}
}

static void	free_cylinder_list(t_cylinder *cylinder_list)
{
	t_cylinder	*cylinder;

	while (cylinder_list)
	{
		cylinder = cylinder_list;
		cylinder_list = cylinder_list->next;
		free(cylinder);
	}
}
