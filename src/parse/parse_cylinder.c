#include "minirt.h"

void	parse_cylinder(t_scene *scene, char **line)
{
	t_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		free_exit(scene, ERR_ALLOC);
	cylinder->next = scene->cylinder_list;
	scene->cylinder_list = cylinder;
	if (!read_v3(line, &cylinder->coords))
		free_exit(scene, ERR_CYL_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &cylinder->axis))
		free_exit(scene, ERR_CYL_AXIS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double_positive(line, &cylinder->diameter))
		free_exit(scene, ERR_CYL_DIAMETER);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double_positive(line, &cylinder->height))
		free_exit(scene, ERR_CYL_HEIGHT);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &cylinder->color))
		free_exit(scene, ERR_CYL_COLOR);
}
