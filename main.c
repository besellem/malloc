/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:19:54 by besellem          #+#    #+#             */
/*   Updated: 2022/03/29 16:49:37 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#if 1
# include "malloc.h"
#else
# include <unistd.h>
# include <stdlib.h>
#endif

int		main(void)
{
	const int			pagesize = getpagesize();

	printf("pagesize:   [%8d]\n", pagesize);
	printf("BLOCK_SIZE: [%8zu]\n", BLOCK_SIZE);
	printf("ZONE_TINY:  [%8zu]\n", ZONE_TINY);
	printf("ZONE_SMALL: [%8zu]\n", ZONE_SMALL);

	const char			str[] = "Hello World!";
	const size_t		size = sizeof(str);
	
	__unused void		*ptr = malloc(65500);
	print_blocks();
	__unused void		*ptr2 = malloc(65449);
	print_blocks();

	// free(ptr);
	__unused void		*ptr3 = malloc(16000);
	print_blocks();
	__unused void		*ptr4 = malloc(524264);
	print_blocks();

	if (NULL == ptr)
	{
		dprintf(STDERR_FILENO, "Error: malloc(%zu)\n", size);
		return (1);
	}

	// print_blocks();
	
	ptr = realloc(ptr, 65537);
	print_blocks();

	ptr = realloc(ptr, 0);
	free(ptr);
	ptr = realloc(ptr, 1000);
	ptr = realloc(ptr, 1);
	ptr = realloc(ptr, 3493049);
	// print_blocks();


	show_alloc_mem();

	free(ptr);
	// free(ptr2);
	// free(ptr2);
	// free(ptr3);
	// free(ptr4);
	show_memory_leaks(true);

	return (0);
}


// int		main(__unused int ac, __unused char **av)
// {
// 	__unused void	*tester_variable = malloc(sizeof(int));

// 	show_alloc_mem();
// 	free(tester_variable);
// 	free(tester_variable);
// 	show_memory_leaks(true);
// 	return (0);
// }
