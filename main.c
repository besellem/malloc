/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:19:54 by besellem          #+#    #+#             */
/*   Updated: 2021/11/02 01:27:51 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "incs/malloc.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const size_t		_sz = sizeof(unsigned long long);
	unsigned long long	*l_src;
	unsigned long long	*l_dst;
	char				*c_src;
	char				*c_dst;

	if (dst == src)
		return (dst);
	l_src = (unsigned long long *)src;
	l_dst = (unsigned long long *)dst;
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

#define HEX  "0123456789abcdef"
// PRINT_MEMORY
static void	ft_print_hex2(unsigned char nb)
{
	write(1, &HEX[nb / 16], 1);
	write(1, &HEX[nb % 16], 1);
}

static void	ft_putnbr_hex(unsigned long nbr)
{
	unsigned long	div;

	div = 16;
	while (nbr / div >= 16)
		div *= 16;
	while (div > 0)
	{
		write(1, &HEX[nbr / div], 1);
		nbr %= div;
		div /= 16;
	}
}

static void	ft_print_hex(char *str, unsigned int *size)
{
	unsigned int	i;
	unsigned int	len;

	i = 0;
	len = 0;
	while (i < 16 && i < *size)
	{
		ft_print_hex2(str[i]);
		if ((len += 2) && i % 2 == 1 && ++len)
			write(1, " ", 1);
		++i;
	}
	while (++len <= 40)
		write(1, " ", 1);
	i = 0;
	while (i < 16 && i < *size)
	{
		if (str[i] < 32 || str[i] == 127)
			write(1, ".", 1);
		else
			write(1, &str[i], 1);
		++i;
	}
	*size -= i;
}

void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	len;
	int				i;
	char			*str;

	if (size == 0)
		return (addr);
	str = (char *)addr;
	len = 0;
	while (len < size)
	{
		if ((len++ % 16) == 0 || size < 16)
		{
			i = 64;
			while ((i -= 8) >= 0)
				ft_putnbr_hex((((unsigned long)str + i) >> i) % 256);
			write(1, ": ", 2);
			ft_print_hex(str, &size);
			write(1, "\n", 1);
			str += 16;
			len = 0;
		}
	}
	return (addr);
}
// END PRINT_MEMORY



int	main(void)
{
	struct rlimit	lim;
	int				ret = getrlimit(RLIMIT_MEMLOCK, &lim);
	const int		pagesize = getpagesize();

	printf("pagesize: [%d]\n", pagesize);
	printf("pagesize tiny:  [%8d] ZONE_TINY:  [%8d]\n",  TINY * pagesize, ZONE_TINY);
	printf("pagesize small: [%8d] ZONE_SMALL: [%8d]\n", SMALL * pagesize, ZONE_SMALL);
	printf("pagesize large: [%8d] ZONE_LARGE: [%8d]\n", LARGE * pagesize, ZONE_LARGE);
	
	// printf("ret: %d\n", ret);
	// printf("cur: [%llu]  max: [%llu]\n", lim.rlim_cur, lim.rlim_max);


	const char	str[] = "Hello World!";
	size_t		size = sizeof(str);
	void		*ptr = NULL;

	ptr = malloc(size);
	if (NULL == ptr)
	{
		dprintf(STDERR_FILENO, "Error: malloc(%zu)\n", size);
		return (1);
	}
	printf("ptr [%p]\n", ptr);

	// ft_print_memory(ptr - sizeof(block_t), size + sizeof(block_t));

	ft_memcpy(ptr, str, size);
	// dprintf(STDOUT_FILENO, "[%p] [%s]\n", ptr, ptr);
	ft_print_memory(ptr - sizeof(block_t), size + sizeof(block_t));

	// printf("offset: [%zu]\n", __offsetof(block_t, _in_use));
	
	free(ptr);
	// ft_print_memory(ptr - sizeof(block_t), size + sizeof(block_t));
	free(ptr);
	return (0);
}
