#include "minirt.h"

static void	init(t_scene	*scene);

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

static void	init(t_scene	*scene)
{
	scene->mlx = mlx_init(WIDTH, HEIGHT, "MiniRT", false);
	if (!scene->mlx)
		free_exit(scene, OK);
	scene->img = mlx_new_image(scene->mlx, WIDTH, HEIGHT);
	if (!scene->img)
		free_exit(scene, ERR_MLX_IMG);
	update_camera_axis(scene->camera);
}
