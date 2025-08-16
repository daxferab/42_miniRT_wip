#include "minirt.h"

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
