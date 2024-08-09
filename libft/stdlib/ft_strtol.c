/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:22:34 by nkannan           #+#    #+#             */
/*   Updated: 2024/05/19 10:47:21 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// ——About strtol——
// If it overflows, LONG_MAX or LONG_MIN is returned.

// static int	ft_isspace(char c)
// {
// 	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
// 		|| c == ' ')
// 		return (1);
// 	return (0);
// }

// Function to determine if 'num' will overflow
static int	ft_is_overflow(unsigned long long num, int sign, unsigned int digit)
{
	if (num > LONG_MAX / 10 || (num == LONG_MAX / 10 && digit > LONG_MAX % 10))
	{
		if (sign == 1)
			return (1);
		else
			return (-1);
	}
	return (0);
}

static long	handle_overflow(int sign)
{
	if (sign == 1)
		return (LONG_MAX);
	else
		return (LONG_MIN);
}

static int	get_digit(char c, int base)
{
	if (ft_isdigit(c))
		return (c - '0');
	else if (base > 10 && ft_isalpha(c))
		return (ft_tolower(c) - 'a' + 10);
	return (-1);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	unsigned long long	num;
	int					sign;
	int					digit;

	num = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		sign *= -1;
	if (*str == '-' || *str == '+')
		str++;
	digit = get_digit(*str, base);
	while (*str && digit >= 0)
	{
		if (digit >= base || ft_is_overflow(num, sign, digit))
			return (handle_overflow(sign));
		num = num * base + digit;
		str++;
		digit = get_digit(*str, base);
	}
	if (endptr)
		*endptr = (char *)str;
	return (num * sign);
}
