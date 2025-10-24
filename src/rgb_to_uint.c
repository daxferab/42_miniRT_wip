#include "minirt.h"

uint32_t	rgb_to_uint(t_color *color)
{
	return (color->red << 24 | color->green << 16 | color->blue << 8 | 255);
}
