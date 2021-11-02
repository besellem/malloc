/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2021/11/02 01:26:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

void	free(void *ptr)
{
	block_t		*block;

	printf(GREEN ">" CLR " IN FREE\n");
	
	if (NULL == ptr)
		return ;
	block = (block_t *)(ptr - sizeof(block_t));
	dprintf(STDOUT_FILENO, "in use: [%d] total_size [%zu]\n",
		block->_in_use, block->_size + sizeof(block_t));

	// ft_print_memory(block, sizeof(block_t));

	if (BLOCK_FREED == block->_in_use)
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n", ptr);
	}
	else
	{

#ifndef MALLOC_DEBUG

		munmap(ptr, block->_size + sizeof(block_t));

#else

		if ((-1) == munmap(ptr, block->_size + sizeof(block_t)))
		{
			dprintf(STDERR_FILENO, RED "Error: " CLR "munmap(%p, %zu)\n",
				block, block->_size + sizeof(block_t));
		}

#endif /* MALLOC_DEBUG */

		block->_in_use = BLOCK_FREED;
	}
	printf(GREEN "<" CLR " IN FREE\n");
}
