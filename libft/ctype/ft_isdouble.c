/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdouble.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 23:47:58 by nkannan           #+#    #+#             */
/*   Updated: 2024/05/03 19:35:55 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static bool	check_format(char *str, bool *is_dot, bool *has_digit)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '.')
		{
			if (*is_dot)
				return (false);
			*is_dot = true;
		}
		else if (ft_isdigit(str[i]))
			*has_digit = true;
		else
			return (false);
		i++;
	}
	return (true);
}

bool	ft_isdouble(char *str)
{
	bool	is_dot;
	bool	has_digit;

	is_dot = false;
	has_digit = false;
	if (str == NULL || *str == '\0')
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	return (check_format(str, &is_dot, &has_digit) && has_digit);
}
