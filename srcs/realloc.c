/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:49 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 09:39:55 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

static void	*_realloc_internal(void *ptr, size_t size)
{
	const size_t	_size_needed = get_sanitized_size(size);
	void			*new_ptr = NULL;
	block_t			*block;

	if (NULL == ptr)
		return (_malloc_internal(size));

	
	/* implementation specific (on mac, frees the pointer) */
	if (0 == size)
	{
		_free_internal(ptr, FREE_INTERNAL_DEBUG(ptr));
		return (NULL);
	}

	block = get_ptr_meta(ptr);
	
	/*
	** if the pointer has been freed before, the behavior is undefined (C11 standard, chapter §7.22.3.5)
	** I chose to display an error and return NULL since the user is not supposed to use a freed pointer.
	*/
	if (BLOCK_FREED == block->_status)
	{
		ft_putstr_fd(STDERR_FILENO,
			BLUE "Warning:" CLR " Attempting realloc() on a freed pointer\n");
		return (NULL);
	}
	
	if ((block->_size - BLOCK_SIZE) >= _size_needed)
	{
		split_block(block, _size_needed);
		return (ptr);
	}
	
	if (block->_next &&
		block->_next->_status == BLOCK_FREED &&
		(block->_size + block->_next->_size) >= _size_needed &&
		block->_zone == block->_next->_zone &&
		DIFF((size_t)block, (size_t)block->_next) == block->_size)
	{
		// trick
		block->_status = BLOCK_FREED;
		defragment_blocks();
		block->_status = BLOCK_IN_USE;
		
		split_block(block, _size_needed);
		return (ptr);
	}
	else
	{
		new_ptr = _malloc_internal(size);
		if (!new_ptr)
			return (NULL);
		ft_memcpy(new_ptr, ptr, block->_size - BLOCK_SIZE);
		_free_internal(ptr, FREE_INTERNAL_DEBUG(ptr));
		return (new_ptr);
	}

	return (ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void	*new_ptr = NULL;

	MLOG("realloc()");

	pthread_mutex_lock(&g_malloc_mutex);
	new_ptr = _realloc_internal(ptr, size);
	pthread_mutex_unlock(&g_malloc_mutex);

	return (new_ptr);
}
