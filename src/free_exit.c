#include "minirt.h"

void	free_exit(t_scene *scene, t_error error)
{
	if (error != OK)
		ft_printf("Error\n%i\n", error);
	//TODO free everything in scene
	free(scene);
	exit(error);
}
