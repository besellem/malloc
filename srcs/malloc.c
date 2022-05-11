/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:41 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 15:07:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

pthread_mutex_t	g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


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

// add zone sorted by zone 
static void		_add_zone(block_t *new_zone)
{
	block_t	*last = last_block(); // current last block

	if (!new_zone)
		return ;
	
	if (NULL == *first_block())
		*first_block() = new_zone;

	/* add new_zone to the list */
	if (last)
		last->_next = new_zone;

	// block_t	*zone;
	// block_t	*next;
	// block_t	*prev;

	// if (!new_zone)
	// 	return ;
	// zone = _next_zone(true);
	// prev = NULL;
	// while (zone && (size_t)new_zone < (size_t)zone)
	// {
	// 	prev = zone;
	// 	zone = _next_zone(false);
		
	// 	// get last block in zone
	// 	while (prev && prev->_next && prev->_next != zone)
	// 		prev = prev->_next;
	// }
	// if (!zone)
	// {
	// 	// push back
	// 	if (*first_block())
	// 		last_block()->_next = new_zone;
	// 	else
	// 		*first_block() = new_zone;
	// }
	// else if (!prev)
	// {
	// 	// push front
	// 	new_zone->_next = *first_block();
	// 	*first_block() = new_zone;
	// }
	// else
	// {
	// 	// push middle
	// 	next = prev->_next;
	// 	prev->_next = new_zone;
		
	// 	block_t *newz_last_block = new_zone;
	// 	while (newz_last_block->_next)
	// 		newz_last_block = newz_last_block->_next;
	// 	newz_last_block->_next = next;
	// }
}

/* find a block that is free and that */
static block_t	*_find_free_block(size_t size)
{
	block_t	*block = *first_block();

	for ( ; block != NULL; block = block->_next)
	{
		if (BLOCK_FREED == block->_status && block->_size > size)
		{
			split_block(block, size);
			return (block);
		}
	}
	return (NULL);
}

static void	_init_zone(block_t *block, size_t zone_size)
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

static block_t	*_create_zone(size_t size)
{
	const size_t	zone_size = define_block_size(size);
	block_t			*block = mmap(last_block(), zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (MAP_FAILED == block)
	{
#ifdef MALLOC_DEBUG
		ft_putstr_fd(STDERR_FILENO, RED "Error:" CLR " mmap(");
		ft_putnbr_fd(STDERR_FILENO, zone_size, 0);
		ft_putstr_fd(STDERR_FILENO, ")\n");
#endif
		return (NULL);
	}

	_init_zone(block, zone_size);
	split_block(block, size);
	_add_zone(block);
	return (block);
}

NOEXPORT
void	*_malloc_internal(size_t size)
{
	const size_t	_size = get_sanitized_size(size);
	block_t			*block;

	if (0 == size)
		return (NULL);
	block = _find_free_block(_size);
	if (!block)
	{
		block = _create_zone(_size);
		if (!block)
			return (NULL);
	}
	return (get_ptr_user(block));
}

void	*malloc(size_t size)
{
	void				*ptr;

	MLOG("malloc()");

	pthread_mutex_lock(&g_malloc_mutex);
	ptr = _malloc_internal(size);
	pthread_mutex_unlock(&g_malloc_mutex);

	return (ptr);
}
