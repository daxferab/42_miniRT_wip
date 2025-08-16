#include "minirt.h"

void	parse_plane(t_scene *scene, char **line)
{
	t_plane	*plane;

	plane = malloc(sizeof(t_plane));
	if (!plane)
		free_exit(scene, ERR_ALLOC);
	if (!scene->plane_list)
		plane->next = NULL;
	else
		plane->next = scene->plane_list;
	scene->plane_list = plane;
	if (!read_v3(line, &plane->coords))
		free_exit(scene, ERR_PLA_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &plane->normal))
		free_exit(scene, ERR_PLA_NORMAL);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &plane->color))
		free_exit(scene, ERR_PLA_COLOR);
}
