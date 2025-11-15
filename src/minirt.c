#include "minirt.h"

static void	init(t_scene *scene);
static void	init_camera(t_scene *scene, t_vector orientation);

int	main(int argc, char **argv)
{
	t_scene	*scene;

	scene = ft_calloc(1, sizeof(t_scene));
	if (!scene)
		free_exit(scene, ERR_ALLOC);
	if (argc != 2)
		free_exit(scene, ERR_ARGUMENTS);
	parse_file(scene, argv[1]);
	init(scene);
	mlx_close_hook(scene->mlx, &close_hook, scene);
	mlx_key_hook(scene->mlx, &key_hook, scene);
	mlx_loop_hook(scene->mlx, &loop_hook, scene);
	mlx_loop(scene->mlx);
	return (0);
}

static void	init(t_scene *scene)
{
	scene->mlx = mlx_init(WIDTH, HEIGHT, "MiniRT", false);
	if (!scene->mlx)
		free_exit(scene, OK);
	scene->img = mlx_new_image(scene->mlx, WIDTH, HEIGHT);
	if (!scene->img)
		free_exit(scene, ERR_MLX_IMG);
	scene->world_east = v3_build(1, 0, 0);
	scene->world_up = v3_build(0, 1, 0);
	scene->world_south = v3_build(0, 0, 1);
	init_camera(scene, scene->camera->orientation);
}

static void	init_camera(t_scene *scene, t_vector orientation)
{
	if (fabs(orientation.y) == 1)
	{
		scene->camera->pitch = M_PI / 2 * orientation.y;
		scene->camera->yaw = -M_PI / 2;
	}
	else
	{
		scene->camera->pitch = asin(orientation.y);
		scene->camera->yaw = atan2(orientation.z, orientation.x);
	}
	scene->camera->pitch_to_change = 0;
	scene->camera->yaw_to_change = 0;
	update_camera_axis(scene, scene->camera);
}
