#include "minirt.h"

static void	free_plane_list(t_plane *plane_list);
static void	free_sphere_list(t_sphere *sphere_list);
static void	free_cylinder_list(t_cylinder *cylinder_list);

static const char	*g_message[MAX_ERR_CODE] = {
	"",
	"Could not allocate memory",
	"There must be one and only one argument",
	"The scene file must have .rt extension",
	"Could not open file",
	"Invalid or lonely identifier",
	"Elements of a scene must be separated by one or more line breaks",
	"Each type of information must be separated by one or more spaces",
	"Ambient lighting is not declared",
	"Ambient lighting is declared several times",
	"Ambient ratio must be a double in the range [0.0,1.0]",
	"Ambient color must be 3 integers the range [0,255] split by commas",
	"Camera is not declared",
	"Camera is declared several times",
	"Camera coordinates must be 3 doubles split by commas",
	"Camera orientation normalized vector must be 3 doubles split by commas",
	"Camera FOV must be an integer in the range [0,180]",
	"Light is not declared",
	"Light is declared several times",
	"Light coordinates must be 3 doubles split by commas",
	"Light ratio must be a double in the range [0.0,1.0]",
	"Plane coordinates must be 3 doubles split by commas",
	"Plane normal normalized vector must be 3 doubles split by commas",
	"Plane color must be 3 integers the range [0,255] split by commas",
	"Sphere coordinates must be 3 doubles split by commas",
	"Sphere diameter must be a positive double",
	"Sphere color must be 3 integers the range [0,255] split by commas",
	"Cylinder coordinates must be 3 doubles split by commas",
	"Cylinder axis normalized vector must be 3 doubles split by commas",
	"Cylinder diameter must be a positive double",
	"Cylinder height must be a positive double",
	"Cylinder color must be 3 integers the range [0,255] split by commas",
	"Could not create the window",
};

void	free_exit(t_scene *scene, t_error error)
{
	if (error != OK)
		ft_printf("Error\n%s\n", g_message[error]);
	if (scene->line)
		ft_printf("On line: %s\n", scene->line);
	free(scene->ambient);
	free(scene->camera);
	free(scene->light);
	free_plane_list(scene->plane_list);
	free_sphere_list(scene->sphere_list);
	free_cylinder_list(scene->cylinder_list);
	free(scene->line);
	if (scene->mlx)
		mlx_terminate(scene->mlx);
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
