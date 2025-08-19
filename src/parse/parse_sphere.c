#include "minirt.h"

void	parse_sphere(t_scene *scene, char **line)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		free_exit(scene, ERR_ALLOC);
	sphere->next = scene->sphere_list;
	scene->sphere_list = sphere;
	if (!read_v3(line, &sphere->coords))
		free_exit(scene, ERR_SPH_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double_positive(line, &sphere->diameter))
		free_exit(scene, ERR_SPH_DIAMETER);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &sphere->color))
		free_exit(scene, ERR_SPH_COLOR);
}
