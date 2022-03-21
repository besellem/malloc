/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_memory_leaks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:44:34 by besellem          #+#    #+#             */
/*   Updated: 2022/03/21 15:49:50 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	_free_all_blocks(void)
{
	block_t	*block;

	for (block = *first_block(); block != NULL; )
	{
		if (BLOCK_FREED != block->_status)
		{
			free(get_ptr_user(block));
			block = *first_block();
		}
		else
			block = block->_next;
	}
}

void	show_memory_leaks(bool free_all)
{
	const block_t	*block = *first_block();
	size_t			sanitized_leaks = 0;
	size_t			real_leaks = 0;

	for ( ; block != NULL; block = block->_next)
	{
		if (BLOCK_IN_USE == block->_status)
		{
			printf(RED "* mem leak" CLR " -> %11p (%zu bytes)\n",
				block, block->_size - BLOCK_SIZE);
			sanitized_leaks += block->_size - BLOCK_SIZE;
			real_leaks += block->_size;
		}
	}

	if (free_all)
		_free_all_blocks();

	if (0 == real_leaks)
		printf(GREEN "*** No memory leaks ***" CLR "\n");
	else
	{
		printf(RED "  Total memory leaked" CLR "  -> %zu bytes\n", sanitized_leaks);
		printf(RED "  Total used by mmap()" CLR " -> %zu bytes\n", real_leaks);
	}
}
