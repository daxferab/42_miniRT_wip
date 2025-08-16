#include "minirt.h"

void	parse_ambient(t_scene *scene, char **line)
{
	if (scene->ambient)
		free_exit(scene, ERR_AMB_SEVERAL);
	scene->ambient = malloc(sizeof(t_ambient));
	if (!scene->ambient)
		free_exit(scene, ERR_ALLOC);
	if (!read_double_ratio(line, &scene->ambient->ratio))
		free_exit(scene, ERR_AMB_RATIO);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &scene->ambient->color))
		free_exit(scene, ERR_AMB_COLOR);
}
