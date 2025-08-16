#include "minirt.h"

void	parse_camera(t_scene *scene, char **line)
{
	if (scene->camera)
		free_exit(scene, ERR_CAM_SEVERAL);
	scene->camera = malloc(sizeof(t_camera));
	if (!scene->camera)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->camera->coords))
		free_exit(scene, ERR_CAM_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &scene->camera->orientation))
		free_exit(scene, ERR_CAM_ORIENTATION);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_int_maxed(line, &scene->camera->fov, INT_MAX_FOV))
		free_exit(scene, ERR_CAM_FOV);
}
