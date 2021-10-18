/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:06:03 by besellem          #+#    #+#             */
/*   Updated: 2021/10/18 10:30:12 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc_internal.h"
#define __BYTE 8

typedef unsigned long long		t_ullong;

static t_ullong	get_c_long_aligned(unsigned char c, size_t l_size)
{
	const t_ullong	c_ = (t_ullong)c;
	t_ullong		c_ret;

	c_ret = 0;
	while (l_size > 0)
	{
		--l_size;
		c_ret |= (c_ << (__BYTE * l_size));
	}
	return (c_ret);
}

void	*ft_memset(void *b, int c, size_t len)
{
	const size_t	l_size = sizeof(t_ullong);
	t_ullong		c_long_aligned;
	t_ullong		*l_ptr;
	unsigned char	*c_ptr;

	c = (unsigned char)c;
	if (0 == c)
		c_long_aligned = 0;
	else
		c_long_aligned = get_c_long_aligned(c, l_size);
	l_ptr = (t_ullong *)b;
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