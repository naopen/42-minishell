/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 06:49:37 by nkannan           #+#    #+#             */
/*   Updated: 2024/04/21 23:41:24 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static double	check_inf_nan(const char *str, int sign)
{
	if (ft_strncmp(str, "inf", 3) == 0 || ft_strncmp(str, "infinity", 8) == 0)
		return (INFINITY * sign);
	if (ft_strncmp(str, "nan", 3) == 0 || ft_strncmp(str, "NaN", 3) == 0)
		return (NAN);
	return (0);
}

static char	*move_space_char(char *str, int *sign)
{
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		*sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	return (str);
}

static char	*after_dot(const char *str, double fraction, double *result)
{
	while (ft_isdigit(*str))
	{
		fraction *= 0.1;
		*result += (*str - '0') * fraction;
		str++;
	}
	return ((char *)str);
}

static double	after_exp(const char *str, double result)
{
	int	exp_sign;
	int	exponent;

	exp_sign = 1;
	exponent = 0;
	if (*str == '-')
	{
		exp_sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (ft_isdigit(*str))
		exponent = exponent * 10 + (*str++ - '0');
	while (exponent--)
	{
		if (exp_sign > 0)
			result *= 10.0;
		else
			result *= 0.1;
	}
	return (result);
}

double	ft_atof(const char *str)
{
	double	result;
	double	fraction;
	int		sign;

	result = 0.0;
	fraction = 1.0;
	sign = 1;
	if (str == NULL)
		return (0);
	str = move_space_char((char *)str, &sign);
	if (check_inf_nan(str, sign))
		return (check_inf_nan(str, sign));
	while (ft_isdigit(*str))
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.')
		str++;
	str = after_dot(str, fraction, &result);
	if (*str == 'e' || *str == 'E')
		result = after_exp(++str, result);
	return (sign * result);
}
