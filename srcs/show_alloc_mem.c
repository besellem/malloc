/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:46:32 by besellem          #+#    #+#             */
/*   Updated: 2022/03/29 16:48:55 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdbool.h>

static void	_show_alloc_mem_wrapper(void)
{
	block_t		*block = *first_block();
	int			zone;
	bool		zone_changed = true;

	if (!block)
		return ;

	zone = block->_zone;
	for ( ; block->_next; block = block->_next)
	{
		if (zone_changed)
		{
			if (block->_zone == MASK_ZONE_TINY)
				printf("TINY: %p\n", block);
			else if (block->_zone == MASK_ZONE_SMALL)
				printf("SMALL: %p\n", block);
			else if (block->_zone == MASK_ZONE_LARGE)
				printf("LARGE: %p\n", block);
			zone_changed = false;
		}
		zone_changed = (zone != block->_zone);
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
