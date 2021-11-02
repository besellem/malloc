/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:41 by besellem          #+#    #+#             */
/*   Updated: 2021/11/02 01:26:17 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

// void	*malloc(size_t size)
// {
// 	return (realloc(NULL, size));
// }


// block_t	*find_block(size_t size)
// {
// 	// if (size <= TINY)
// 	// {
		
// 	// }
// 	// else 
// }

void	*malloc(size_t size)
{
	block_t		*block;

	printf(GREEN ">" CLR " IN MALLOC\n");

	if (0 == size)
		return (NULL);
	// if (size <= TINY)
	// {
		
	// }
	// else if (size <= SMALL)
	// {
		
	// }
	// else
	// {
	// 	block = create_block(size);
	// 	if (!block)
	// 		return (NULL);	
	// }

	block = mmap(NULL,
				 size + sizeof(block_t),
				 PROT_READ | PROT_WRITE,
				 MAP_PRIVATE | MAP_ANONYMOUS,
				 -1, 0);
	if (MAP_FAILED == block)
	{
#ifndef MALLOC_DEBUG
		dprintf(STDERR_FILENO, "Error: mmap(%p, %zu)\n", block, size + sizeof(block_t));
#endif /* MALLOC_DEBUG */
		return (NULL);
	}
	block->_in_use = BLOCK_IN_USE;
	block->_size = size;

	ft_print_memory(block, size + sizeof(block_t));
	printf("in use: [%d] size [%zu]\n", block->_in_use, block->_size);
	
	// block = find_block(size);
	// if (!block)
	// {
	// 	block = create_block(size);
	// 	if (!block)
	// 		return (NULL);
	// }
	printf(GREEN "<" CLR " IN MALLOC\n");
	return ((void *)block + sizeof(block_t));
}
