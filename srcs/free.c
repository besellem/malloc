/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/15 13:24:44 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"


/*
** Deallocate all empty zones at the end of the program
*/
__attribute__((destructor))
void	deallocate_empty_zones(void)
{
	block_t		*zone = get_next_zone(true);
	block_t		*next_zone = NULL;
	block_t		*prev_zone = NULL;
	block_t		*last_from_prev_zone = NULL;
	block_t		*_block = NULL;

	size_t		zone_size;
	bool		zone_empty;

	MLOG("_deallocate_empty_zones()");

	for ( ; zone != NULL; zone = next_zone)
	{
		next_zone = get_next_zone(false);

		zone_size = 0;
		zone_empty = true;
		for (_block = zone; _block && _block->_next != next_zone; _block = _block->_next)
		{
			zone_size += _block->_size;

			if (zone_empty && !_block->_next)
				break ;

			if (BLOCK_IN_USE == _block->_status)
				zone_empty = false;
		}
		zone_size += _block->_size; // add zone's last block size

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

#ifdef MALLOC_DEBUG
			ft_putstr_fd(STDERR_FILENO, BLUE "munmap(");
			ft_putaddr_fd(STDERR_FILENO, zone, 0);
			ft_putstr_fd(STDERR_FILENO, ", ");
			ft_putnbr_fd(STDERR_FILENO, zone_size, 0);
			ft_putstr_fd(STDERR_FILENO, ")" CLR "\n");
#endif

			if (munmap(zone, zone_size) != 0)
			{
#ifdef MALLOC_DEBUG
				ft_putstr_fd(STDERR_FILENO, RED "Error: " CLR "munmap(");
				ft_putaddr_fd(STDERR_FILENO, zone, 0);
				ft_putstr_fd(STDERR_FILENO, ", ");
				ft_putnbr_fd(STDERR_FILENO, zone_size, 0);
				ft_putstr_fd(STDERR_FILENO, ")\n");
#endif
			}

#ifdef MALLOC_VERBOSE
			MLOG("zone deallocated");
			print_blocks();
#endif

			next_zone = get_next_zone(true);
		}
		else
		{
			prev_zone = zone;
			last_from_prev_zone = _block;
		}
	}
#ifdef MALLOC_VERBOSE
	print_blocks();
#endif
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

void	_free_internal(void *ptr)
{
	block_t	*block;

	if (NULL == ptr)
		return ;

	block = get_ptr_meta(ptr);

	/*
	** this may also be a double free in case its zone is already freed.
	** unfortunately, we can't check if it existed in the first place.
	*/
	if (!_find_block(block))
	{
		ft_putstr_fd(STDERR_FILENO,
			RED "Error:" CLR " Pointer " GREEN);
		ft_putaddr_fd(STDERR_FILENO, ptr, 0);
		ft_putstr_fd(STDERR_FILENO, CLR " was not allocated\n");
		return ;
	}

	/*
	** if there's no allocated blocks left or if the pointer has been freed,
	** must be a double free since the pointer is not NULL.
	*/
	if (BLOCK_FREED == block->_status)
	{
		ft_putstr_fd(STDERR_FILENO,
			BLUE "Warning:" CLR " Attempting double free on address ");
		ft_putaddr_fd(STDERR_FILENO, ptr, 0);
		ft_putstr_fd(STDERR_FILENO, "\n");
		return ;
	}

	block->_status = BLOCK_FREED;
	// deallocate_empty_zones();
	defragment_blocks();
}

void	free(void *ptr)
{
	MLOG("free()");

	pthread_mutex_lock(&g_malloc_mutex);
	_free_internal(ptr);
	pthread_mutex_unlock(&g_malloc_mutex);
}
