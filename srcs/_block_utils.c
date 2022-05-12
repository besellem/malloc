/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _block_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 12:09:00 by besellem          #+#    #+#             */
/*   Updated: 2022/05/12 13:00:10 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

/*
** Get first block
*/
block_t	**first_block(void)
{
	static block_t	*block = NULL;

	return (&block);
}

/*
** Get last block
*/
block_t	*last_block(void)
{
	block_t	*block = *first_block();

	while (block && block->_next != NULL)
		block = block->_next;
	return (block);
}

/*
** Split a block into two blocks
*/
void	split_block(block_t *block, size_t size)
{
	typeof(*block)	old_blk;

	ft_memcpy(&old_blk, block, sizeof(old_blk));
	block->_status = BLOCK_IN_USE;
	block->_size = size;
	block->_next = (void *)block + size;
	block->_next->_zone = old_blk._zone;
	block->_next->_status = BLOCK_FREED;
	block->_next->_size = DIFF(size, old_blk._size);
	block->_next->_next = old_blk._next;
}

/*
** Join all contiguous freed blocks on all zones (defragmentation)
*/
void	defragment_blocks(void)
{
	block_t	*zone = get_next_zone(true);
	block_t	*next_zone = zone;

	while (zone && next_zone)
	{
		next_zone = get_next_zone(false);

		for (block_t *block = zone; block && block != next_zone && block->_next && block->_next != next_zone; )
		{
			if (BLOCK_FREED == block->_status && BLOCK_FREED == block->_next->_status)
			{
				const block_t *next_block = block->_next;
				
				if (BLOCK_FREED == block->_status && BLOCK_FREED == next_block->_status)
				{
					block->_size += next_block->_size;
					block->_next = next_block->_next;
				}
				block = zone;
			}
			else
				block = block->_next;
		}
		zone = next_zone;
	}
}

/*
** Find the next zone
** Useful to loop through all zones
**
** Zones are already sorted by address, no need to sort them
*/
block_t	*get_next_zone(bool reset)
{
	static block_t	*block = NULL;
	int				zone;

	if (reset || !block)
	{
		block = *first_block();
		return (block);
	}
	zone = block->_zone;
	for ( ; block && block->_next; block = block->_next)
	{
		if (DIFF((size_t)block, (size_t)block->_next) != block->_size)
		{
			block = block->_next;
			return (block);
		}
		if (zone != block->_zone)
			return (block);
		if (BLOCK_SIZE == block->_size)
		{
			block = block->_next;
			return (block);
		}
	}
	return (NULL);
}
