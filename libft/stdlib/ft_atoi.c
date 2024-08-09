/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 08:38:20 by nkannan           #+#    #+#             */
/*   Updated: 2024/05/19 10:48:49 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Convert a string to an integer.
// If the string is not a valid integer, return 0.
// Integers can start with an optional '+' or '-' sign.
// Integers can contain digits only.
// Convert the string until the first non-digit character.

int	ft_atoi(const char *str)
{
	return ((int)ft_strtol(str, NULL, 10));
}
