/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:26:21 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 18:48:44 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Fill the first n bytes of the memory area
// pointed to by buf with the constant byte ch.
// Equivalent to memset with the second argument being ch.
// Returns the pointer buf.

void	*ft_memset(void *buf, int ch, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)buf;
	while (n--)
		*ptr++ = (unsigned char)ch;
	return (buf);
}
