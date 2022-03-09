/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:41 by besellem          #+#    #+#             */
/*   Updated: 2022/03/09 13:45:09 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

block_t	**first_block(void)
{
	static block_t	*block = NULL;

	return (&block);
}

block_t	**last_block(void)
{
	static block_t	*block;

	block = *first_block();
	if (block)
	{
		for ( ; block->_next != NULL; block = block->_next);
	}
	return (&block);
}

void	split_block(block_t *block, size_t size)
{
	const size_t	old_size = block->_size;
	block_t			*old_block = block->_next;

	block->_status = BLOCK_IN_USE;
	block->_size = size;
	block->_next = (void *)block + size;
	block->_next->_status = BLOCK_FREED;
	block->_next->_size = MAX(old_size, size) - MIN(old_size, size);
	block->_next->_next = old_block;
}

void	add_block(block_t *block)
{
	block_t	*last = *last_block(); // current last block

	if (!block)
		return ;
	
	if (NULL == *first_block())
		*first_block() = block;

	/* add new block to the list */
	if (last)
		last->_next = block;
}

block_t	*find_block(size_t size)
{
	block_t	*block = *first_block();

	for ( ; block; block = block->_next)
	{
		if (BLOCK_FREED == block->_status && block->_size >= size)
		{
			split_block(block, size);
			return (block);
		}
	}
	return (NULL);
}

void	_print_blocks(void)
{
	const block_t	*blk = *first_block();

	printf(GREEN "..................BLOCKS.................." CLR "\n");
	printf(CYAN "    addr    |    next    |   size   | used | zone" CLR "\n");

	for ( ; blk; blk = blk->_next)
	{
		printf(" %11p  %11p  %9zu %4u %6u\n",
			blk, blk->_next, blk->_size, blk->_status, blk->_zone);
	}
	
	printf(GREEN "................BLOCKS.END................" CLR "\n");
}

block_t	*create_block(size_t size)
{
	const size_t	_siz = define_block_size(size);
	block_t			*block = mmap(NULL, _siz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (MAP_FAILED == block)
	{
#ifndef MALLOC_DEBUG
		dprintf(STDERR_FILENO, RED "Error:" CLR " mmap(%zu)\n", _siz);
#endif /* MALLOC_DEBUG */
		return (NULL);
	}

	if (_siz == ZONE_TINY)
		block->_zone = MODE_ZONE_TINY;
	else if (_siz == ZONE_SMALL)
		block->_zone = MODE_ZONE_SMALL;
	else
		block->_zone = MODE_ZONE_LARGE;

	block->_size = _siz;
	block->_next = NULL;
	split_block(block, size);
	add_block(block);
	return (block);
}

static void	*_malloc_wrapper(size_t size)
{
	const size_t	_size = align(size + BLOCK_SIZE);
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
