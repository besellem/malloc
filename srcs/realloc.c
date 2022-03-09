/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:49 by besellem          #+#    #+#             */
/*   Updated: 2022/03/09 13:37:45 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*_realloc_wrapper(void *ptr, size_t size)
{
	const size_t		_size_needed = align(size + BLOCK_SIZE);
	void				*new_ptr = NULL;
	block_t				*block;

	if (NULL == ptr)
		return (malloc(size));
	
	// implementation specific (on mac, frees the pointer)
	if (0 == size)
	{
		free(ptr);
		return (ptr);
	}

	block = get_ptr_meta(ptr);
	if ((block->_size - BLOCK_SIZE) >= _size_needed)
	{	
		split_block(block, _size_needed);
		return (ptr);
	}
	
	if (block->_next &&
		block->_next->_status == BLOCK_FREED &&
		(block->_size + block->_next->_size) > _size_needed)
	{
		block->_status = BLOCK_FREED;
		join_blocks();
		block->_status = BLOCK_IN_USE;
		split_block(block, _size_needed);
		return (ptr);
	}
	else
	{
		new_ptr = malloc(size);
		if (!new_ptr)
			return (NULL);
		ft_memcpy(new_ptr, ptr, block->_size - BLOCK_SIZE);
		free(ptr);
		return (new_ptr);
	}
	
	return (ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void				*new_ptr = NULL;
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	new_ptr = _realloc_wrapper(ptr, size);
	
	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);

	return (new_ptr);
}
