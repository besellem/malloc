/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:46:32 by besellem          #+#    #+#             */
/*   Updated: 2022/05/09 11:35:11 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"
#include <stdbool.h>
#include <inttypes.h>

// static void	_show_alloc_mem_wrapper(void)
// {
// 	static const char	*_zone_name[] = {
// 		"TINY",
// 		"SMALL",
// 		"LARGE"
// 	};
// 	block_t		*block = *first_block();
// 	int			zone;
// 	bool		zone_changed = true;

// 	if (!block)
// 		return ;

// 	/*
// 	** trick to set `zone_changed' to true the first time so it prints the first zone.
// 	** the first condition will systematically be true the first time
// 	*/
// 	zone = !block->_zone;
// 	for ( ; block->_next; block = block->_next)
// 	{
// 		zone_changed = (zone != block->_zone) || (0 == block->_size) || \
// 						DIFF((size_t)block, (size_t)block->_next) != block->_size;
// 		if (zone_changed)
// 		{
// 			printf("%s: %p\n", _zone_name[block->_zone], block);
// 			zone_changed = false;
// 		}
// 		zone = block->_zone;
// 		// if (block->_size > 0)
// 			printf("  %p - %p : %zu bytes\n", block, block->_next, block->_size);
// 	}
// }

// void     show_alloc_mem(void)
// {
// 	pthread_mutex_t		_m;

// 	pthread_mutex_init(&_m, NULL);
// 	pthread_mutex_lock(&_m);

// 	_show_alloc_mem_wrapper();

// 	pthread_mutex_unlock(&_m);
// 	pthread_mutex_destroy(&_m);
// }

/*
* TODO:
* - the address of the zone must be the first non-zero block of the zone
* - the zones and blocks must be sorted by address
*/

static block_t	*_next_zone(bool reset)
{
	static block_t	*block = NULL;
	int				zone;
	bool			zone_changed;

	if (reset)
	{
		block = *first_block();
		return (block);
	}
	block = block->_next;
	zone = block->_zone;
	for ( ; block && block->_next; block = block->_next)
	{
		zone_changed = (zone != block->_zone) || (0 == block->_size) || \
						DIFF((size_t)block, (size_t)block->_next) != block->_size;
		if (zone_changed)
			return (block);
	}
	return (NULL);
}

// static
block_t	*_next_sorted_zone(void)
{
	static block_t	*last_min = NULL;
	block_t			*min = NULL;
	block_t			*zone = NULL;

	zone = _next_zone(true);
	if (!last_min)
	{
		// last_min = min = zone;
		// last_min = zone;
		min = zone;
	}
	for ( ; zone; zone = _next_zone(false))
	{
		if (min != last_min && (size_t)min > (size_t)zone)
		{
			min = zone;
		}
	}
	last_min = min;
	return (min);
}

static void	_show_alloc_mem_wrapper(void)
{
	static const char	*_zone_name[] = {
		"TINY",
		"SMALL",
		"LARGE"
	};
	// block_t				*zone = _next_sorted_zone();
	// // block_t				*block = NULL;

	// for ( ; zone; zone = _next_sorted_zone())
	// {
	// 	printf("%s: %p\n", _zone_name[zone->_zone], zone);
	// 	// print_blocks();

	// 	// for (block = zone; block->_next; block = block->_next)
	// 	// {
	// 	// 	printf("  %p - %p : %zu bytes\n", block, block->_next, block->_size);
	// 	// }
	// }


	block_t		*block = *first_block();
	int			zone;
	bool		zone_changed;

	if (!block)
		return ;

	/*
	** trick to set `zone_changed' to true the first time so it prints the first zone.
	** the first condition will systematically be true the first time
	*/
	zone = !block->_zone;
	for ( ; block->_next; block = block->_next)
	{
		zone_changed = (zone != block->_zone) || (0 == block->_size) || \
						DIFF((size_t)block, (size_t)block->_next) != block->_size;
		if (zone_changed)
		{
			printf("%s: %p\n", _zone_name[block->_zone], block);
			zone_changed = false;
		}
		zone = block->_zone;
		// if (block->_size > 0)
			printf("  %p - %p : %zu bytes\n", block, block->_next, block->_size);
	}
}

void     show_alloc_mem(void)
{
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	_show_alloc_mem_wrapper();

	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);
}
