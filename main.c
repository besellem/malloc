/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:19:54 by besellem          #+#    #+#             */
/*   Updated: 2022/05/15 13:38:10 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "malloc_internal.h" // print_blocks()
#include "malloc.h"

#ifndef __unused
# define __unused __attribute__((unused))
#endif


int		main(void)
{
	// printf("pagesize:   [%8d]\n", getpagesize());
	// printf("BLOCK_SIZE: [%8zu]\n", BLOCK_SIZE);
	// printf("ZONE_TINY:  [%8zu]\n", ZONE_TINY);
	// printf("ZONE_SMALL: [%8zu]\n", ZONE_SMALL);

	// print_blocks();
	__unused void		*ptr = malloc(65490);
	// print_blocks();
	
	__unused void		*ptr2 = malloc(65449);
	// print_blocks();

	// free(ptr);
	__unused void		*ptr3 = malloc(16000);
	// print_blocks();
	
	__unused void		*ptr4 = malloc(524264);
	// print_blocks();

	__unused void		*ptr5 = malloc(2147483647UL * 2147483647UL);
	// print_blocks();

	__unused void		*ptr6 = malloc(sizeof(char));
	// print_blocks();


	ptr = realloc(ptr, 65537);
	// print_blocks();

	ptr = realloc(ptr, 0);
	// print_blocks();
	
	free(ptr);
	// print_blocks();
	
	ptr = realloc(ptr, 1000);
	// print_blocks();

	ptr = realloc(ptr, 1);
	// print_blocks();
	
	ptr = realloc(ptr, 3493049);
	// print_blocks();

	// print_blocks();
	show_alloc_mem();

	free(ptr);
	// print_blocks();

	free(ptr2);
	// print_blocks();

	free(ptr2);
	// print_blocks();
	
	free(ptr3);
	// print_blocks();

	free(ptr4);
	// print_blocks();

	free(ptr5);
	// print_blocks();

	free(ptr6);
	free(ptr6);
	
	free((void *)0x1024);

	// print_blocks();
	show_memory_leaks();

	return (0);
}

// int		main(void)
// {
// 	__unused void	*tester_variable = malloc(sizeof(int));

// 	show_alloc_mem();
// 	free(tester_variable);
// 	show_alloc_mem();
// 	free(tester_variable);
	
// 	show_memory_leaks();
// 	return (0);
// }
