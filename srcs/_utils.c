/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 00:00:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/09 15:01:38 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

/*
** MEMSET
*/
static wide_int_t	get_aligned_char(unsigned char c, size_t l_size)
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
		c_long_aligned = get_aligned_char(c, l_size);
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

static void	_print_padding(int fd, int n, char c)
{
	while (n-- > 0)
		write(fd, &c, 1);
}

#define HEXA "0123456789abcdef"
static void	ft_putaddr_fd(const void *addr, int fd, int pad)
{
	size_t	ad = (size_t)addr;
	size_t	div = 16;

	if (!addr)
		_print_padding(fd, pad - 4, ' ');
	else
		_print_padding(fd, pad - 14, ' ');
	
	write(fd, "0x", 2);
	while (ad / div >= 16)
		div *= 16;
	while (div > 0)
	{
		write(fd, &HEXA[ad / div], 1);
		ad %= div;
		div /= 16;
	}
}

inline void	ft_putaddr(const void *addr, int pad)
{
	ft_putaddr_fd(addr, STDOUT_FILENO, pad);
}

static size_t	ft_strlen(const char *str)
{
	const char	*p = str;

	if (!str)
		return (0);
	while (*p)
		++p;
	return ((size_t)p - (size_t)str);
}

void	ft_putstr(const char *s)
{
	if (s)
		write(STDOUT_FILENO, s, ft_strlen(s));
}

void	ft_putnstr(char *s, size_t n)
{
	const size_t	len = ft_strlen(s);

	if (s)
	{
		if (n > len)
			n = len;
		write(STDOUT_FILENO, s, n);
	}
}

static int	ft_nblen_base(long long n, int base)
{
	int	len;

	if (n < 0)
		n = -n;
	len = 1;
	while (n / base > 0)
	{
		n /= base;
		++len;
	}
	return (len);
}

static void	_putnbr(int n)
{
	long	nb;
	long	tmp;

	nb = (long)n;
	if (nb < 0)
	{
		nb = -nb;
		write(STDOUT_FILENO, "-", 1);
	}
	if (nb / 10 > 0)
		_putnbr(nb / 10);
	tmp = nb % 10 + 48;
	write(STDOUT_FILENO, &tmp, 1);
}

void	ft_putnbr(int n, int pad)
{
	const int	_pad = DIFF(pad, ft_nblen_base(n, 10));

	_print_padding(STDOUT_FILENO, _pad, ' ');
	_putnbr(n);
}
