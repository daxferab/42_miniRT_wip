#include "minirt.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
		free_exit(ERR_ARGUMENTS);
	parse_file(argv[1]);
}
