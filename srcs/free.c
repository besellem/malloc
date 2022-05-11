/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 09:55:23 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

/*
** Join all contiguous freed blocks on all zones (defragmentation)
*/
void	defragment_blocks(void)
{
	block_t	*zone = _next_zone(true);
	block_t	*next_zone = zone;

	while (zone && next_zone)
	{
		next_zone = _next_zone(false);
		for (block_t *block = zone; block && block != next_zone && block->_next && block->_next != next_zone; )
		{
			if (BLOCK_FREED == block->_status && BLOCK_FREED == block->_next->_status)
			{
				const block_t *next_block = block->_next;
				
				if (BLOCK_FREED == block->_status && BLOCK_FREED == next_block->_status)
				{
					block->_size += next_block->_size;
					block->_next = next_block->_next;
				}
				block = zone;
			}
			else
				block = block->_next;
		}
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
				ft_putstr_fd(STDERR_FILENO, RED "Error: " CLR "munmap(");
				ft_putaddr_fd(zone, STDERR_FILENO, 0);
				ft_putstr_fd(STDERR_FILENO, ", ");
				ft_putnbr_fd(STDERR_FILENO, zone_size, 0);
				ft_putstr_fd(STDERR_FILENO, ")\n");
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

static int	_find_block(const block_t *block)
{
	for (block_t *tmp = *first_block(); tmp != NULL; tmp = tmp->_next)
	{
		if (tmp == block)
			return (1);
	}
	return (0);
}

static void	_print_debug_info(const struct s_debug_data *debug)
{
#ifdef MALLOC_DEBUG
	ft_putstr_fd(STDERR_FILENO, debug->file);
	ft_putstr_fd(STDERR_FILENO, ":");
	ft_putnbr_fd(STDERR_FILENO, debug->line, 0);
	ft_putstr_fd(STDERR_FILENO, ": " GREEN);
	ft_putstr_fd(STDERR_FILENO, debug->ptr_name);
	ft_putstr_fd(STDERR_FILENO, CLR "\n");
#else
	(void)debug;
	ft_putstr_fd(STDERR_FILENO,
		"define " CYAN "MALLOC_DEBUG" CLR " to get more informations\n");
#endif
}

NOEXPORT
void	_free_internal(void *ptr, const struct s_debug_data *debug)
{
	block_t	*block;

	if (NULL == ptr)
		return ;

	/*
	** if there's no allocated blocks left or if the pointer has been freed,
	** must be a double free since the pointer is not NULL
	*/
	block = get_ptr_meta(ptr);

	// this may also be a double free, but it's zone is already freed
	if (!_find_block(block))
	{
		ft_putstr_fd(STDERR_FILENO,
			RED "Error:" CLR " Pointer " GREEN);
		ft_putaddr_fd(debug->ptr, STDERR_FILENO, 0);
		ft_putstr_fd(STDERR_FILENO, CLR " was not allocated\n       ");
		_print_debug_info(debug);
		return ;
	}

	if (!*first_block() || BLOCK_FREED == block->_status)
	{
		ft_putstr_fd(STDERR_FILENO,
			BLUE "Warning:" CLR " Attempting double free on address ");
		ft_putaddr_fd(debug->ptr, STDERR_FILENO, 0);
		ft_putstr_fd(STDERR_FILENO, "\n         ");
		_print_debug_info(debug);
		return ;
	}

	block->_status = BLOCK_FREED;
	deallocate_empty_zones();
	defragment_blocks();
}

/* a wrapper to handle threads and debug infos */
void	_free_wrapper(void *ptr, const struct s_debug_data *debug)
{
	MLOG("free()");

	pthread_mutex_lock(&g_malloc_mutex);
	_free_internal(ptr, debug);
	pthread_mutex_unlock(&g_malloc_mutex);
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
	_free_wrapper(ptr, FREE_INTERNAL_DEBUG(ptr));
}
#endif
