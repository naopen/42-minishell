/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 09:48:26 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/20 17:37:42 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Allocate a block of size bytes from the heap memory,
// and fill it with zeros.
// Returns a pointer to the allocated memory.

// If count or size is 0,
// returns NULL or a unique pointer value
// that can later be successfully passed to free().

// ——About malloc——
// If it overflows, is not detected,
// a memory block of incorrect size will be allocated.

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (!count || !size)
	{
		count = 1;
		size = 1;
	}
	if (count > __SIZE_MAX__ / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
