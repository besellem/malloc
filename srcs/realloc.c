/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:49 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:07:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

// static void	_init(malloc_t *p)
// {
// 	ft_memset(p, 0, sizeof(malloc_t));
// #ifdef THREAD_SAFE
// 	pthread_mutex_init(&p->_m, NULL);
// 	pthread_mutex_lock(&p->_m);
// #endif /* THREAD_SAFE */
// }

// static void	_end(malloc_t *p)
// {
// #ifdef THREAD_SAFE
// 	pthread_mutex_unlock(&p->_m);
// 	pthread_mutex_destroy(&p->_m);
// #endif /* THREAD_SAFE */
// 	ft_memset(p, 0, sizeof(malloc_t));
// }

void	*realloc(void *ptr, size_t size)
{
	void	*new_ptr = NULL;
	block_t	*block;

	if (NULL == ptr)
		return (malloc(size));
	
	print_blocks();

	block = get_ptr_meta(ptr);
	if (!block->_next ||
		BLOCK_IN_USE == block->_next->_status ||
		(block->_size + block->_next->_size) < size)
	{
		new_ptr = malloc(size);
		if (!new_ptr)
			return (NULL);
		ft_memcpy(new_ptr, ptr, block->_size - BLOCK_SIZE);
		free(ptr);
		return (new_ptr);
	}

	/* case where we realloc a lower size than ptr size */
	
	// if (size < block->_size)
	// 	split_block(block, size);
	// else
		split_block(block, align(size + BLOCK_SIZE));

	print_blocks();

	return (ptr);
}
