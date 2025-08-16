#include "minirt.h"

bool	skip_spaces(char **line)
{
	if (**line != ' ')
		return (false);
	while (**line == ' ')
		(*line)++;
	return (true);
}
