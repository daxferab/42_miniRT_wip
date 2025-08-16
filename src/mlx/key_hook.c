#include "minirt.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_scene	*scene;

	scene = param;
	if (keydata.key == MLX_KEY_ESCAPE)
		free_exit(scene, OK);
}
