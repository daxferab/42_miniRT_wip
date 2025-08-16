#include "minirt.h"

void	close_hook(void *param)
{
	t_scene	*scene;

	scene = param;
	free_exit(scene, OK);
}
