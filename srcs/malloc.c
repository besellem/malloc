/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:41 by besellem          #+#    #+#             */
/*   Updated: 2022/05/09 11:41:33 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"

block_t	**first_block(void)
{
	static block_t	*block = NULL;

	return (&block);
}

block_t	*last_block(void)
{
	block_t	*block = *first_block();

	while (block && block->_next != NULL)
		block = block->_next;
	return (block);
}

// TO REMOVE
void	_print_blocks_wrapper(void)
{
	block_t	*blk = *first_block();

	printf(GREEN "..........................BLOCKS............................" CLR "\n");
	printf(CYAN "      addr      |      next      |   size   | used |  zone  " CLR "\n");

	for ( ; blk != NULL; blk = blk->_next)
	{
		printf(" %15p  %15p  %9zu %4u ",
			blk, blk->_next, blk->_size, blk->_status);
		
		if (blk->_zone == MASK_ZONE_TINY)
			printf("%9s\n", "tiny");
		else if (blk->_zone == MASK_ZONE_SMALL)
			printf("%9s\n", "small");
		else if (blk->_zone == MASK_ZONE_LARGE)
			printf("%9s\n", "large");
		else
			printf(RED"%9s"CLR"\n", "unknown");
	}
	printf(GREEN "........................BLOCKS.END.........................." CLR "\n");
}
// END - TO REMOVE

void	split_block(block_t *block, size_t size)
{
	block_t	old_blk;

	ft_memcpy(&old_blk, block, sizeof(*block));
	block->_status = BLOCK_IN_USE;
	block->_size = size;
	block->_next = (void *)block + size;
	block->_next->_zone = old_blk._zone;
	block->_next->_status = BLOCK_FREED;
	block->_next->_size = DIFF(size, old_blk._size);
	block->_next->_next = old_blk._next;
}

static void		add_block(block_t *block)
{
	block_t	*last = last_block(); // current last block

	if (!block)
		return ;
	
	if (NULL == *first_block())
		*first_block() = block;

	/* add new block to the list */
	if (last)
		last->_next = block;
}

/* find a block that is free and that */
static block_t	*find_block(size_t size)
{
	block_t	*block = *first_block();

	for ( ; block != NULL; block = block->_next)
	{
		if (BLOCK_FREED == block->_status && block->_size >= size)
		{
			split_block(block, size);
			return (block);
		}
	}
	return (NULL);
}

static void	_init_block(block_t *block, size_t zone_size)
{
	if (zone_size <= ZONE_TINY)
		block->_zone = MASK_ZONE_TINY;
	else if (zone_size <= ZONE_SMALL)
		block->_zone = MASK_ZONE_SMALL;
	else
		block->_zone = MASK_ZONE_LARGE;

	block->_status = BLOCK_IN_USE;
	block->_size = zone_size;
	block->_next = NULL;
}

static block_t	*create_block(size_t size)
{
	const size_t	zone_size = define_block_size(size);
	block_t			*block = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (MAP_FAILED == block)
	{
#ifdef MALLOC_DEBUG
		dprintf(STDERR_FILENO, RED "Error:" CLR " mmap(%zu)\n", zone_size);
#endif
		return (NULL);
	}

	_init_block(block, zone_size);
	split_block(block, size);
	add_block(block);
	return (block);
}

static void	*_malloc_wrapper(size_t size)
{
	const size_t	_size = get_sanitized_size(size);
	block_t			*block;

	if (0 == size)
		return (NULL);
	block = find_block(_size);
	if (!block)
	{
		block = create_block(_size);
		if (!block)
			return (NULL);
	}
	return (get_ptr_user(block));
}

void	*malloc(size_t size)
{
	void				*ptr;
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	ptr = _malloc_wrapper(size);
	
	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);

	return (ptr);
}
