#include "minirt.h"

void	parse_light(t_scene *scene, char **line)
{
	if (scene->light)
		free_exit(scene, ERR_LIG_SEVERAL);
	scene->light = malloc(sizeof(t_light));
	if (!scene->light)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->light->coords))
		free_exit(scene, ERR_LIG_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double_ratio(line, &scene->light->ratio))
		free_exit(scene, ERR_LIG_RATIO);
}
