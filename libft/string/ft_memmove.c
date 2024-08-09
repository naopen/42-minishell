/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 08:41:27 by nkannan           #+#    #+#             */
/*   Updated: 2024/03/23 04:32:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Copies len bytes from string src to string dst.
// The two strings may overlap;
// the copy is always done in a non-destructive manner.
// Returns the original value of dst.

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_c;
	unsigned char	*src_c;

	dst_c = (unsigned char *)dst;
	src_c = (unsigned char *)src;
	if (dst_c == NULL && src_c == NULL)
		return (NULL);
	if (dst_c > src_c)
	{
		while (len--)
			dst_c[len] = src_c[len];
	}
	else
		ft_memcpy(dst_c, src_c, len);
	return (dst);
}
