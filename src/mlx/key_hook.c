#include "minirt.h"

static void	key_hook_move(mlx_key_data_t keydata, t_scene *scene);
static void	key_hook_rotate(mlx_key_data_t keydata, t_scene *scene);

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_scene	*scene;

	scene = param;
	if (keydata.key == MLX_KEY_ESCAPE)
		free_exit(scene, OK);
	key_hook_move(keydata, scene);
	key_hook_rotate(keydata, scene);
}

static void	key_hook_move(mlx_key_data_t keydata, t_scene *scene)
{
	if ((keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_S && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->world_south);
	if ((keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_W && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->world_south);
	if ((keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->world_east);
	if ((keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->world_east);
	if ((keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_LEFT_SHIFT && keydata.action == MLX_RELEASE))
		scene->movement = v3_add(scene->movement, scene->world_up);
	if ((keydata.key == MLX_KEY_LEFT_SHIFT && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE))
		scene->movement = v3_substract(scene->movement, scene->world_up);
}

static void	key_hook_rotate(mlx_key_data_t keydata, t_scene *scene)
{
	if ((keydata.key == MLX_KEY_UP && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_DOWN && keydata.action == MLX_RELEASE))
		scene->camera->pitch_to_change += PITCH_CHANGE;
	if ((keydata.key == MLX_KEY_DOWN && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_UP && keydata.action == MLX_RELEASE))
		scene->camera->pitch_to_change -= PITCH_CHANGE;
	if ((keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_RELEASE))
		scene->camera->yaw_to_change += YAW_CHANGE;
	if ((keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS)
		|| (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_RELEASE))
		scene->camera->yaw_to_change -= YAW_CHANGE;
}
