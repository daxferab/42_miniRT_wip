#include "minirt.h"

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
