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

void	parse_ambient(t_scene *scene, char **line)
{
	scene->ambient = malloc(sizeof(t_ambient));
	if (!scene->ambient)
		free_exit(scene, ERR_ALLOC);
	if (!read_double(line, &scene->ambient->ratio))
		ft_printf("bad double\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_color(line, &scene->ambient->color))
		ft_printf("bad color\n");
}

void	parse_camera(t_scene *scene, char **line)
{
	scene->camera = malloc(sizeof(t_camera));
	if (!scene->camera)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->camera->coords))
		ft_printf("bad coords\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_v3(line, &scene->camera->orientation))
		ft_printf("bad orientation\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_int_maxed(line, &scene->camera->fov, INT_MAX_FOV))
		ft_printf("bad fov\n");
}

void	parse_light(t_scene *scene, char **line)
{
	scene->light = malloc(sizeof(t_light));
	if (!scene->light)
		free_exit(scene, ERR_ALLOC);
	if (!read_v3(line, &scene->light->coords))
		ft_printf("bad coords\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_double(line, &scene->light->ratio))
		ft_printf("bad ratio\n");
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
		ft_printf("bad coords\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_v3(line, &plane->normal))
		ft_printf("bad nomal\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_color(line, &plane->color))
		ft_printf("bad color\n");
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
		ft_printf("bad coords\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_double(line, &sphere->diameter))
		ft_printf("bad diameter\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_color(line, &sphere->color))
		ft_printf("bad color\n");
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
		ft_printf("bad coords\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_v3(line, &cylinder->axis))
		ft_printf("bad axis\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_double(line, &cylinder->diameter))
		ft_printf("bad diameter\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_double(line, &cylinder->height))
		ft_printf("bad height\n");
	if (!skip_spaces(line))
		ft_printf("bad spaces\n");
	if (!read_color(line, &cylinder->color))
		ft_printf("bad color\n");
}

void	parse_line(t_scene *scene, char *line)
{
	void	(*parse_type)(t_scene *, char **);

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
	{
		ft_printf("bad identifier\n");
		return ;
	}
	skip_identifier(&line);
	parse_type(scene, &line);
	if (*line != '\n' && *line != '\0')
		ft_printf("bad ending %s\n", line);
}
