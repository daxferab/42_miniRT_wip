#include "minirt.h"

static void	check_extension(t_scene *scene, char *path);
static void	open_file(t_scene *scene, char *path, int *fd);

void	parse_file(t_scene *scene, char *path)
{
	int		fd;

	check_extension(scene, path);
	open_file(scene, path, &fd);
	while (true)
	{
		scene->line = ft_get_next_line(fd);
		if (!scene->line)
			break ;
		if (!ft_str_equals(scene->line, "\n"))
			parse_line(scene, scene->line);
		free(scene->line);
	}
	if (!scene->ambient)
		free_exit(scene, ERR_AMB_NONE);
	if (!scene->camera)
		free_exit(scene, ERR_CAM_NONE);
	if (!scene->light)
		free_exit(scene, ERR_LIG_NONE);
}

static void	check_extension(t_scene *scene, char *path)
{
	size_t	len_path;
	size_t	len_ext;
	char	*extension;

	len_path = ft_strlen(path);
	len_ext = ft_strlen(FILE_EXTENSION);
	if (len_path <= len_ext)
		free_exit(scene, ERR_EXTENSION);
	extension = ft_substr(path, len_path - len_ext, len_ext);
	if (!extension)
		free_exit(scene, ERR_ALLOC);
	if (!ft_str_equals(extension, FILE_EXTENSION))
	{
		free(extension);
		free_exit(scene, ERR_EXTENSION);
	}
	free(extension);
	if (path[len_path - len_ext - 1] == PATH_SEPARATOR)
		free_exit(scene, ERR_EXTENSION);
}

static void	open_file(t_scene *scene, char *path, int *fd)
{
	errno = 0;
	*fd = open(path, O_RDWR);
	if (errno == EISDIR)
		free_exit(scene, ERR_OPEN);
	close(*fd);
	*fd = open(path, O_RDONLY);
	if (*fd < 0)
		free_exit(scene, ERR_OPEN);
}
