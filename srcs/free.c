/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2022/05/10 16:31:44 by besellem         ###   ########.fr       */
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

static void	_print_debug_info(const struct s_debug_data debug)
{
#ifdef MALLOC_DEBUG
	ft_putstr_fd(STDERR_FILENO, debug.file);
	ft_putstr_fd(STDERR_FILENO, ":");
	ft_putnbr_fd(STDERR_FILENO, debug.line, 0);
	ft_putstr_fd(STDERR_FILENO, ": " GREEN);
	ft_putstr_fd(STDERR_FILENO, debug.ptr_name);
	ft_putstr_fd(STDERR_FILENO, CLR "\n");
#else
	(void)debug;
	ft_putstr_fd(STDERR_FILENO,
		"define " CYAN "MALLOC_DEBUG" CLR " to get more informations\n");
#endif
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


	// this may also be a double free, but it's zone is already freed
	if (!_find_block(block))
	{
		ft_putstr_fd(STDERR_FILENO,
			RED "Error:" CLR " Pointer " GREEN);
		ft_putaddr_fd(debug.ptr, STDERR_FILENO, 0);
		ft_putstr_fd(STDERR_FILENO, CLR " was not allocated\n       ");
		_print_debug_info(debug);
		return ;
	}
	
	if (!*first_block() || BLOCK_FREED == block->_status)
	{
		ft_putstr_fd(STDERR_FILENO,
			BLUE "Warning:" CLR " Attempting double free on address ");
		ft_putaddr_fd(debug.ptr, STDERR_FILENO, 0);
		ft_putstr_fd(STDERR_FILENO, "\n         ");
		_print_debug_info(debug);
		return ;
	}

	block->_status = BLOCK_FREED;
	deallocate_empty_zones();
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
