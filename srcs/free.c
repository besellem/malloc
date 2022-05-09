/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/10 01:11:04 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"

/*
** frees everything at the end of the program automatically
*/
__attribute__((destructor)) void	_free_all_blocks(void)
{
	block_t	*block = *first_block();

	while (block)
	{
		if (BLOCK_FREED != block->_status && block->_size > 0)
		{
			free(get_ptr_user(block));
			block = *first_block();
		}
		else
			block = block->_next;
	}
}

/*
** Join all contiguous freed blocks.
**
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

void	deallocate_empty_zones(void)
{
	block_t		*zone = _next_zone(true);
	block_t		*next_zone = NULL;
	block_t		*prev_zone = NULL;
	block_t		*last_from_prev_zone = NULL;
	block_t		*_block;

	size_t		zone_size;
	bool		zone_empty;

	for ( ; zone != NULL; zone = next_zone)
	{
		next_zone = _next_zone(false);

		zone_size = 0;
		zone_empty = true;
		for (_block = zone; _block && _block->_next != next_zone; _block = _block->_next)
		{
			zone_size += _block->_size;

			if (zone_empty && !_block->_next)
				break ;

			if (_block->_status == BLOCK_IN_USE)
				zone_empty = false;
		}

		if (zone_empty)
		{	
			if (!prev_zone)
			{
				*first_block() = next_zone; // point to the next zone
			}
			else
			{
				prev_zone = zone;
				if (last_from_prev_zone)
					last_from_prev_zone->_next = _block->_next;
			}

			if (SYSCALL_ERR == munmap(zone, zone_size))
			{
#ifdef MALLOC_DEBUG
				dprintf(STDERR_FILENO, RED "Error: " CLR "munmap(%p, %zu)\n",
					zone, zone_size);
#endif
			}

			next_zone = _next_zone(true);
		}
		else
		{
			prev_zone = zone;
			last_from_prev_zone = _block;
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
		ft_putstr_fd(STDERR_FILENO,
			BLUE "Warning:" CLR " Attempting double free on address ");
		ft_putaddr_fd(debug.ptr, STDERR_FILENO, 0);
		ft_putstr_fd(STDERR_FILENO, "\n");

#ifdef MALLOC_DEBUG
		dprintf(STDERR_FILENO, "%9.0d%s:%d: " GREEN "%s" CLR "\n",
			0, debug.file, debug.line, debug.ptr_name);
#endif

		return ;
	}

	block->_status = BLOCK_FREED;
	deallocate_empty_zones();
	// join_blocks();
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
