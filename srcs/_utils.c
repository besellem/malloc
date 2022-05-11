/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 00:00:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 17:21:29 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"


/* Biggest integer size (128 bits or lower) */
#ifdef __SIZEOF_INT128__
typedef __uint128_t            wide_int_t;
#else // !__SIZEOF_INT128__
typedef unsigned long long     wide_int_t;
#endif // __SIZEOF_INT128__


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


/*
** FT_NBLEN_BASE
*/
int	ft_nblen_base(size_t n, int base)
{
	int	len;

	len = 1;
	while (n / base > 0)
	{
		n /= base;
		++len;
	}
	return (len);
}


/*
** FT_PUTADDR
*/
static void	_print_padding(int fd, int n, char c)
{
	while (n-- > 0)
		write(fd, &c, 1);
}

#define HEXA "0123456789abcdef"
void	ft_putaddr_fd(const void *addr, int fd, int pad)
{
	size_t	ad = (size_t)addr;
	size_t	div = 16;

	if (pad > 0)
	{	
		if (!addr)
			_print_padding(fd, pad - 6, ' ');
		else
			_print_padding(fd, DIFF(ft_nblen_base((size_t)addr, 10), pad) - 1, ' ');
	}
	
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

void	ft_putaddr(const void *addr, int pad)
{
	ft_putaddr_fd(addr, STDOUT_FILENO, pad);
}


/*
** FT_STRLEN
*/
static size_t	ft_strlen(const char *str)
{
	const char	*p = str;

	if (!str)
		return (0);
	while (*p)
		++p;
	return ((size_t)p - (size_t)str);
}


/*
** FT_PUTSTR
*/
void	ft_putstr_fd(int fd, const char *s)
{
	if (s)
		write(fd, s, ft_strlen(s));
}

void	ft_putstr(const char *s)
{
	ft_putstr_fd(STDOUT_FILENO, s);
}


/*
** FT_PUTNSTR
*/
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


/*
** FT_PUTNBR
*/
static void	_putnbr(int fd, size_t n)
{
	size_t	tmp;

	if (n / 10 > 0)
		_putnbr(fd, n / 10);
	tmp = n % 10 + 48;
	write(fd, &tmp, 1);
}

void	ft_putnbr_fd(int fd, size_t n, int pad)
{
	const int	_pad = DIFF(pad, ft_nblen_base(n, 10));

	_print_padding(fd, (0 == pad ? 0 : _pad), ' ');
	_putnbr(fd, n);
}

void	ft_putnbr(size_t n, int pad)
{
	ft_putnbr_fd(STDOUT_FILENO, n, pad);
}


/*
** PRINT_BLOCKS
*/
void	_print_blocks(void)
{
	block_t	*blk = *first_block();

	ft_putstr(GREEN "..........................BLOCKS............................" CLR "\n");
	ft_putstr(CYAN "      addr      |      next      |   size   | used |  zone  " CLR "\n");

	for ( ; blk != NULL; blk = blk->_next)
	{
		ft_putstr(" ");
		ft_putaddr(blk, 17);
		ft_putstr("  ");
		ft_putaddr(blk->_next, 17);
		ft_putstr("  ");
		ft_putnbr(blk->_size, 9);
		ft_putstr(" ");
		ft_putnbr(blk->_status, 4);
		ft_putstr("      ");
		
		if (blk->_zone == MASK_ZONE_TINY)
			ft_putstr(" tiny\n");
		else if (blk->_zone == MASK_ZONE_SMALL)
			ft_putstr("small\n");
		else if (blk->_zone == MASK_ZONE_LARGE)
			ft_putstr("large\n");
		else
			ft_putstr("\n");
	}
	ft_putstr(GREEN "........................BLOCKS.END.........................." CLR "\n");
}
