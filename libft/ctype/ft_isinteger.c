/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinteger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 08:31:54 by nkannan           #+#    #+#             */
/*   Updated: 2024/06/13 21:53:45 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isinteger(char *str)
{
	long	num;
	size_t	i;
	char	*endptr;

	i = 0;
	if (str[i] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (ft_strlen(str + i) > 10)
		return (0);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	num = ft_strtol(str, &endptr, 10);
	if (*endptr != '\0' || num > INT_MAX || num < INT_MIN)
		return (0);
	if (*endptr == '\0' && num == 0 && (str[0] == '-' || str[0] == '+'))
		return (0);
	return (1);
}
