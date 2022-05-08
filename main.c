/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:19:54 by besellem          #+#    #+#             */
/*   Updated: 2022/05/08 16:46:32 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#if 1
# include "malloc_internal.h"
# include "malloc.h"
#else
#include <unistd.h>
# include <stdlib.h>
#endif

#ifndef __unused
# define __unused __attribute__((unused))
#endif

// int		main(void)
// {
// 	const int			pagesize = getpagesize();

// 	printf("pagesize:   [%8d]\n", pagesize);
// 	printf("BLOCK_SIZE: [%8zu]\n", BLOCK_SIZE);
// 	printf("ZONE_TINY:  [%8zu]\n", ZONE_TINY);
// 	printf("ZONE_SMALL: [%8zu]\n", ZONE_SMALL);

// 	const char			str[] = "Hello World!";
// 	const size_t		size = sizeof(str);
	
// 	print_blocks();
// 	__unused void		*ptr = malloc(65490);
// 	print_blocks();
	
// 	__unused void		*ptr2 = malloc(65449);
// 	print_blocks();

// 	// free(ptr);
// 	__unused void		*ptr3 = malloc(16000);
// 	print_blocks();
	
// 	__unused void		*ptr4 = malloc(524264);
// 	print_blocks();

// 	__unused void		*ptr5 = malloc(524210);
// 	print_blocks();

// 	__unused void		*ptr6 = malloc(sizeof(char));
// 	print_blocks();

// 	if (NULL == ptr)
// 	{
// 		dprintf(STDERR_FILENO, "Error: malloc(%zu)\n", size);
// 		return (1);
// 	}

// 	// print_blocks();
	
// 	ptr = realloc(ptr, 65537);
// 	print_blocks();

// 	ptr = realloc(ptr, 0);
// 	free(ptr);
// 	ptr = realloc(ptr, 1000);
// 	ptr = realloc(ptr, 1);
// 	ptr = realloc(ptr, 3493049);
// 	// print_blocks();


// 	show_alloc_mem();
// 	print_blocks();

// 	free(ptr);
// 	free(ptr2);
// 	// free(ptr2);
// 	free(ptr3);
// 	free(ptr4);
// 	free(ptr5);
// 	free(ptr6);
// 	// show_memory_leaks(true);

// 	return (0);
// }


int		main(void)
{
	// __unused void	*tester_variable = malloc(sizeof(int));

	// show_alloc_mem();
	// free(tester_variable);
	// free(tester_variable);

	print_blocks();
	show_memory_leaks(false);
	return (0);
}
