/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 13:09:46 by nkannan           #+#    #+#             */
/*   Updated: 2024/03/23 04:32:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Compare byte string s1 against byte string s2.
// Both strings are assumed to be n bytes long.
// Return zero if the two strings are identical,
// otherwise return the difference between the first two differing bytes
// (treated as unsigned char values, so that `\200' is greater than `\0',
// for example).

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	int				diff;

	if (!n)
		return (0);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	n++;
	while (--n)
	{
		diff = *str1 - *str2;
		if (diff != 0)
			return (diff);
		str1++;
		str2++;
	}
	return (0);
}
