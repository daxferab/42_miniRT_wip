#include "minirt.h"

void	free_exit(t_error error)
{
	ft_printf("Error\n%i\n", error);
	exit(error);
}
