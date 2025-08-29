#include "minirt.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_scene	*scene;

	scene = param;
	if (keydata.key == MLX_KEY_ESCAPE)
		free_exit(scene, OK);
	if ((keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_S && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->camera->forward);
	if ((keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_W && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->camera->forward);
	if ((keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->camera->rightward);
	if ((keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->camera->rightward);
	if ((keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_LEFT_SHIFT && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->world_up);
	if ((keydata.key == MLX_KEY_LEFT_SHIFT && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->world_up);
}
