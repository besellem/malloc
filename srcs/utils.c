/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 00:00:21 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:04:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc_internal.h"

/*
** MEMSET
*/
static wide_int_t	get_char_aligned(unsigned char c, size_t l_size)
{
	const wide_int_t	c_ = (wide_int_t)c;
	wide_int_t			c_ret;

	c_ret = 0;
	while (l_size > 0)
	{
		--l_size;
		c_ret |= (c_ << (8 * l_size));
	}
	return (c_ret);
}

void	*ft_memset(void *b, int c, size_t len)
{
	const size_t	l_size = sizeof(wide_int_t);
	wide_int_t		c_long_aligned;
	wide_int_t		*l_ptr;
	unsigned char	*c_ptr;

	c = (unsigned char)c;
	if (0 == c)
		c_long_aligned = 0;
	else
		c_long_aligned = get_char_aligned(c, l_size);
	l_ptr = (wide_int_t *)b;
	while (len >= l_size)
	{
		*l_ptr++ = c_long_aligned;
		len -= l_size;
	}
	if (len > 0)
	{
		c_ptr = (unsigned char *)l_ptr;
		while (len--)
			*c_ptr++ = c;
	}
	return (b);
}

/*
** MEMCPY
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const size_t	_sz = sizeof(wide_int_t);
	wide_int_t		*l_src;
	wide_int_t		*l_dst;
	char			*c_src;
	char			*c_dst;

	if (dst == src)
		return (dst);
	l_src = (wide_int_t *)src;
	l_dst = (wide_int_t *)dst;
	while (n >= _sz)
	{
		*l_dst++ = *l_src++;
		n -= _sz;
	}
	if (n > 0)
	{
		c_src = (char *)l_src;
		c_dst = (char *)l_dst;
		while (n--)
			*c_dst++ = *c_src++;
	}
	return (dst);
}
