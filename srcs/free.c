/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/03/09 13:47:15 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	show_memory_leaks(void)
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
			// free(get_ptr_user(block));
		}
	}

	if (0 == real_leaks)
		printf(GREEN "*** No memory leaks ***" CLR "\n");
	else
	{
		printf(RED "  Total memory leaked" CLR "  -> %zu bytes\n", sanitized_leaks);
		printf(RED "  Total used by mmap()" CLR " -> %zu bytes\n", real_leaks);
	}
}

/* join all contiguous blocks freed */
void	join_blocks(void)
{
	block_t	*block = *first_block();
	block_t	*_next;

	if (!block || !block->_next)
		return ;

	while (block->_next)
	{
		if (BLOCK_FREED == block->_status && BLOCK_FREED == block->_next->_status)
		{
			_next = block->_next;
			if (BLOCK_FREED == block->_status && BLOCK_FREED == _next->_status)
			{
				block->_size += _next->_size;
				block->_next = _next->_next;
			}
			block = *first_block();
		}
		else
			block = block->_next;
	}
}

static void	_free_wrapper(void *ptr)
{
	block_t	*block;
	
	if (NULL == ptr)
		return ;

	/* there's no allocated blocks left, must be a double free */
	if (!*first_block())
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n", ptr);
		return ;
	}
	
	block = get_ptr_meta(ptr);
	if (BLOCK_FREED == block->_status)
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n", ptr);
	}
	else
	{
		block->_status = BLOCK_FREED;
		join_blocks();

#ifdef MALLOC_DEBUG
		printf(CYAN "%p" CLR ": pointer freed\n", ptr);
#endif

		if (*first_block() && NULL == (*first_block())->_next)
		{

#ifndef MALLOC_DEBUG
			munmap(*first_block(), (*first_block())->_size);
#else /* MALLOC_DEBUG */
			if (SYSCALL_ERR == munmap(*first_block(), (*first_block())->_size))
			{
				dprintf(STDERR_FILENO, RED "Error: " CLR "munmap(%p, %zu)\n",
					*first_block(), (*first_block())->_size);
			}
#endif /* !MALLOC_DEBUG */

			*first_block() = NULL;
		}
	}
}

void	free(void *ptr)
{
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	_free_wrapper(ptr);

	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);
}
