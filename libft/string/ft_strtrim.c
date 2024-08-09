/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 14:13:41 by nkannan           #+#    #+#             */
/*   Updated: 2024/03/23 04:32:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// # Description
// Returns a copy of s1 with the characters
// specified in set removed from the beginning and the end of the string.

// # Example
// s1="1234AAA22331122”
// set="1234"
// return="AAA"

// ft_strchr():
// Returns a pointer to the first occurrence of the character c in the string s.
// If the character is not found, the function returns a NULL pointer.
// If c is '\0', the function returns a pointer to the terminator.

// (this can use memcpy...?)

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	len = ft_strlen(s1);
	while (len > i && ft_strchr(set, s1[len - 1]))
		len--;
	trim = (char *)ft_calloc(len - i + 1, sizeof(char));
	if (!trim)
		return (NULL);
	j = 0;
	while (i < len)
		trim[j++] = s1[i++];
	return (trim);
}
