/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:19:54 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:11:31 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#if 1
# include "./incs/malloc.h"
#else
# include <unistd.h>
# include <stdlib.h>
#endif

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
	struct rlimit			lim;
	__unused int			ret = getrlimit(RLIMIT_MEMLOCK, &lim);
	__unused const int		pagesize = getpagesize();

	printf("pagesize:   [%8d]\n", pagesize);
	printf("BLOCK_SIZE: [%8zu]\n", BLOCK_SIZE);
	printf("ZONE_TINY:  [%8zu]\n", ZONE_TINY);
	printf("ZONE_SMALL: [%8zu]\n", ZONE_SMALL);
	
	// printf("ret: %d\n", ret);
	// printf("cur: [%20llu]  max: [%20llu]\n", lim.rlim_cur, lim.rlim_max);
	// printf("cur: [%20llx]  max: [%20llx]\n", lim.rlim_cur, lim.rlim_max);

	const char			str[] = "Hello World!";
	const size_t		size = sizeof(str);
	
	__unused void		*ptr = malloc(size);
	// __unused void		*ptr2 = malloc(size);
	// __unused void		*ptr3 = malloc(16000);
	// __unused void		*ptr4 = malloc(1000);

	if (NULL == ptr)
	{
		dprintf(STDERR_FILENO, "Error: malloc(%zu)\n", size);
		return (1);
	}

	ft_memcpy(ptr, str, size);
	ft_print_memory(get_ptr_meta(ptr), get_ptr_meta(ptr)->_size);

	ptr = realloc(ptr, 10);
	ft_print_memory(get_ptr_meta(ptr), get_ptr_meta(ptr)->_size);

	// ft_memcpy(ptr2, str, size);
	

	// ft_print_memory(*first_block(), (ptrdiff_t)*last_block() - (ptrdiff_t)*first_block());
	
	free(ptr);
	// free(ptr2);
	// free(ptr3);
	// free(ptr4);
	
	ft_check_memory_leaks();

	return (0);
}
