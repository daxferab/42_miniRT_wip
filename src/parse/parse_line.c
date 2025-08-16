#include "minirt.h"

static void	skip_identifier(char **line);

void	parse_line(t_scene *scene, char *line)
{
	void	(*parse_type)(t_scene *, char **);

	parse_type = NULL;
	if (!ft_strncmp(line, "A ", 2))
		parse_type = parse_ambient;
	else if (!ft_strncmp(line, "C ", 2))
		parse_type = parse_camera;
	else if (!ft_strncmp(line, "L ", 2))
		parse_type = parse_light;
	else if (!ft_strncmp(line, "pl ", 3))
		parse_type = parse_plane;
	else if (!ft_strncmp(line, "sp ", 3))
		parse_type = parse_sphere;
	else if (!ft_strncmp(line, "cy ", 3))
		parse_type = parse_cylinder;
	else
		free_exit(scene, ERR_IDENTIFIER);
	skip_identifier(&line);
	parse_type(scene, &line);
	if (*line != '\n' && *line != '\0')
		free_exit(scene, ERR_END_LINE);
}

static void	skip_identifier(char **line)
{
	while (**line != ' ')
		(*line)++;
	while (**line == ' ')
		(*line)++;
}
