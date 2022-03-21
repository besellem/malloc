/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/03/21 16:27:47 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/* join all contiguous freed blocks */
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

static void	_free_wrapper(void *ptr, const struct s_debug_data debug)
{
	block_t	*block;

	if (NULL == ptr)
		return ;

	/*
	** if there's no allocated blocks left or if the pointer has been freed,
	** must be a double free since the pointer is not NULL
	*/
	block = get_ptr_meta(ptr);
	if (!*first_block() || BLOCK_FREED == block->_status)
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n",
			debug.ptr);

#ifdef MALLOC_DEBUG
		dprintf(STDERR_FILENO, "%9.0d%s:%d: " GREEN "%s" CLR " (addr %p)\n",
			0, debug.file, debug.line, debug.ptr_name, debug.ptr);
#endif /* MALLOC_DEBUG */

		return ;
	}

	block->_status = BLOCK_FREED;
	join_blocks();

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

/* a wrapper to handle threads and debug infos */
void	__free(void *ptr, const struct s_debug_data debug)
{
	pthread_mutex_t		_m;// = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	_free_wrapper(ptr, debug);

	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);
}

/*
** free() is defined in two forms :
** - if MALLOC_DEBUG is defined, it's a macro (defined in malloc.h)
**   that calls the wrapper above
** - if not, it's a function (below)
*/
#ifndef MALLOC_DEBUG
void	free(void *ptr)
{
	__free(ptr, (const struct s_debug_data){ ptr, NULL, NULL, 0 });
}
#endif /* MALLOC_DEBUG */
