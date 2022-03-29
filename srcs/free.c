/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/03/29 16:29:10 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/* join all contiguous freed blocks */

/*
** A problem arise in the case of 2 different zones that we want to join :
** If the first is, let say, a ZONE_TINY, and the second is a ZONE_SMALL,
** the join will blend the two, losing track of 'em.
** This will not create a bug per say, but it'll mess up the display of
** show_alloc_mem() function, which is bad.
*/

void	join_blocks(void)
{
	block_t	*block = *first_block();
	block_t	*next;

	if (!block || !block->_next)
		return ;

	while (block && block->_next)
	{
		if (BLOCK_FREED == block->_status && BLOCK_FREED == block->_next->_status)
		{
			next = block->_next;
			if (BLOCK_FREED == block->_status && BLOCK_FREED == next->_status)
			{
				block->_size += next->_size;
				block->_next = next->_next;
			}
			block = *first_block();
		}
		else
			block = block->_next;
	}
}

static
void	_deallocate_empty_zones(void)
{
	block_t		*block = *first_block();
	block_t		*current_zone = NULL;
	block_t		*prev_zone = NULL;
	block_t		*last_from_prev_zone = NULL;

	size_t		zone_size;
	bool		zone_empty;

	/* loop through zones */
	while (block)
	{
	
		/* loop through zone's blocks */
		current_zone = block;
		zone_size = 0;
		zone_empty = true;
		for ( ; block != NULL; block = block->_next)
		{
			if (zone_empty && !block->_next)
			{
				zone_empty = true;
				break ;
			}

			/*
			** if the current block size == 0 or the current and next addresses
			** a have a different size difference, we have changed zone
			*/
			if (0 == block->_size || DIFF((size_t)block, (size_t)block->_next) != block->_size)
				break ;

			zone_size += block->_size;

			if (block->_status == BLOCK_IN_USE)
				zone_empty = false;
		}
		
		/* deallocate zone if it's empty */
		if (zone_empty)
		{
			if (!prev_zone)
			{
				*first_block() = block->_next; // point to the next zone
			}
			else
			{
				prev_zone = current_zone;
				if (last_from_prev_zone)
					last_from_prev_zone->_next = block->_next;
			}

			if (SYSCALL_ERR == munmap(current_zone, zone_size))
			{
#ifdef MALLOC_DEBUG
				dprintf(STDERR_FILENO, RED "Error: " CLR "munmap(%p, %zu)\n",
					current_zone, zone_size);
#endif
			}

			block = *first_block();
		}
		else
		{
			prev_zone = current_zone;
			last_from_prev_zone = block;
			block = block->_next;
		}
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
#endif

		return ;
	}

	block->_status = BLOCK_FREED;
	_deallocate_empty_zones();
	join_blocks();
}

/* a wrapper to handle threads and debug infos */
void	__free(void *ptr, const struct s_debug_data debug)
{
	pthread_mutex_t		_m;

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
#endif
