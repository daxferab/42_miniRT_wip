#include "minirt.h"

void	loop_hook(void *param)
{
	t_scene	*scene;

	scene = param;
	move_camera(scene);
	rotate_camera(scene);
	render(scene);
	if (mlx_image_to_window(scene->mlx, scene->img, 0, 0) == -1)
		free_exit(scene, ERR_MLX_PRINT);
}
