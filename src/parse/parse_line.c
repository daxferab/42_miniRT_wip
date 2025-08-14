#include "minirt.h"

bool	skip_spaces(char **line)
{
	if (**line != ' ')
		return (false);
	while (**line == ' ')
		(*line)++;
	return (true);
}

void	skip_identifier(char **line)
{
	while (**line != ' ')
		(*line)++;
	while (**line == ' ')
		(*line)++;
}

bool	read_double(char **line, double *result)
{
	int		sign;
	double	decimal;

	sign = 1;
	if (**line == '+' || **line == '-')
		if (*(*line)++ == '-')
			sign = -1;
	if (!ft_isdigit(**line))
		return (false);
	*result = 0.0 * sign;
	while (ft_isdigit(**line))
		*result = *result * 10 + (*(*line)++ - '0') * sign;
	if (**line != '.')
		return (true);
	(*line)++;
	if (!ft_isdigit(**line))
		return (false);
	decimal = 1.0;
	while (ft_isdigit(**line))
	{
		decimal /= 10;
		*result = *result + (*(*line)++ - '0') * decimal * sign;
	}
	return (true);
}

bool	read_double_ratio(char **line, double *result)
{
	if (!read_double(line, result))
		return (false);
	if (*result < 0.0 || *result > 1.0)
		return (false);
	return (true);
}

bool	read_int_maxed(char **line, int *result, int max_range)
{
	if (!ft_isdigit(**line))
		return (false);
	*result = 0;
	while (ft_isdigit(**line))
	{
		*result = *result * 10 + *(*line)++ - '0';
		if (*result > max_range)
			return (false);
	}
	return (true);
}

bool	read_color(char **line, t_color *color)
{
	if (!read_int_maxed(line, &color->red, INT_MAX_COLOR))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_int_maxed(line, &color->green, INT_MAX_COLOR))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_int_maxed(line, &color->blue, INT_MAX_COLOR))
		return (false);
	return (true);
}

bool	read_v3(char **line, struct s_v3 *v3)
{
	if (!read_double(line, &v3->x))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_double(line, &v3->y))
		return (false);
	if (*(*line)++ != ',')
		return (false);
	if (!read_double(line, &v3->z))
		return (false);
	return (true);
}

bool	read_v3_normalized(char **line, struct s_v3 *v3)
{
	if (!read_v3(line, v3))
		return (false);
	if (sqrt(pow(v3->x, 2) + pow(v3->y, 2) + pow(v3->z, 2)) != 1)
		return (false);
	return (true);
}

void	parse_ambient(t_scene *scene, char **line)
{
	if (scene->ambient)
		free_exit(scene, ERR_AMB_SEVERAL);
	scene->ambient = malloc(sizeof(t_ambient));
	if (!scene->ambient)
		free_exit(scene, ERR_ALLOC);
	if (!read_double_ratio(line, &scene->ambient->ratio))
		free_exit(scene, ERR_AMB_RATIO);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &scene->ambient->color))
		free_exit(scene, ERR_AMB_COLOR);
}

void	parse_camera(t_scene *scene, char **line)
{
	if (scene->camera)
		free_exit(scene, ERR_CAM_SEVERAL);
	scene->camera = malloc(sizeof(t_camera));
	if (!scene->camera)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->camera->coords))
		free_exit(scene, ERR_CAM_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &scene->camera->orientation))
		free_exit(scene, ERR_CAM_ORIENTATION);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_int_maxed(line, &scene->camera->fov, INT_MAX_FOV))
		free_exit(scene, ERR_CAM_FOV);
}

void	parse_light(t_scene *scene, char **line)
{
	if (scene->light)
		free_exit(scene, ERR_LIG_SEVERAL);
	scene->light = malloc(sizeof(t_light));
	if (!scene->light)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->light->coords))
		free_exit(scene, ERR_LIG_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double_ratio(line, &scene->light->ratio))
		free_exit(scene, ERR_LIG_RATIO);
}

void	parse_plane(t_scene *scene, char **line)
{
	t_plane	*plane;

	plane = malloc(sizeof(t_plane));
	if (!plane)
		free_exit(scene, ERR_ALLOC);
	if (!scene->plane_list)
		plane->next = NULL;
	else
		plane->next = scene->plane_list;
	scene->plane_list = plane;
	if (!read_v3(line, &plane->coords))
		free_exit(scene, ERR_PLA_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &plane->normal))
		free_exit(scene, ERR_PLA_NORMAL);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &plane->color))
		free_exit(scene, ERR_PLA_COLOR);
}

void	parse_sphere(t_scene *scene, char **line)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		free_exit(scene, ERR_ALLOC);
	sphere->next = scene->sphere_list;
	scene->sphere_list = sphere;
	if (!read_v3(line, &sphere->coords))
		free_exit(scene, ERR_SPH_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double(line, &sphere->diameter))
		free_exit(scene, ERR_SPH_DIAMETER);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &sphere->color))
		free_exit(scene, ERR_SPH_COLOR);
}

void	parse_cylinder(t_scene *scene, char **line)
{
	t_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		free_exit(scene, ERR_ALLOC);
	cylinder->next = scene->cylinder_list;
	scene->cylinder_list = cylinder;
	if (!read_v3(line, &cylinder->coords))
		free_exit(scene, ERR_CYL_COORDS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_v3_normalized(line, &cylinder->axis))
		free_exit(scene, ERR_CYL_AXIS);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double(line, &cylinder->diameter))
		free_exit(scene, ERR_CYL_DIAMETER);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_double(line, &cylinder->height))
		free_exit(scene, ERR_CYL_HEIGHT);
	if (!skip_spaces(line))
		free_exit(scene, ERR_SPACES);
	if (!read_color(line, &cylinder->color))
		free_exit(scene, ERR_CYL_COLOR);
}

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
