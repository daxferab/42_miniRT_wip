#include "minirt.h"

static void	check_extension(char *path);
static void	open_file(char *path, int *fd);

void	parse_file(char *path)
{
	char	*line;
	int		fd;

	check_extension(path);
	open_file(path, &fd);
	while (true)
	{
		line = ft_get_next_line(fd);
		if (!line)
			break ;
		parse_line(line);
		free(line);
	}
}

static void	check_extension(char *path)
{
	size_t	len_path;
	size_t	len_ext;
	char	*extension;

	len_path = ft_strlen(path);
	len_ext = ft_strlen(FILE_EXTENSION);
	if (len_path <= len_ext)
		free_exit(ERR_EXTENSION);
	extension = ft_substr(path, len_path - len_ext, len_ext);
	if (!extension)
		free_exit(ERR_ALLOC);
	if (!ft_str_equals(extension, FILE_EXTENSION))
	{
		free(extension);
		free_exit(ERR_EXTENSION);
	}
	free(extension);
	if (path[len_path - len_ext - 1] == PATH_SEPARATOR)
		free_exit(ERR_EXTENSION);
}

static void	open_file(char *path, int *fd)
{
	errno = 0;
	*fd = open(path, O_RDWR);
	if (errno == EISDIR)
		free_exit(ERR_OPEN);
	close(*fd);
	*fd = open(path, O_RDONLY);
	if (*fd < 0)
		free_exit(ERR_OPEN);
}
