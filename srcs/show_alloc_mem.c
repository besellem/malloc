/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:46:32 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 16:47:09 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

// zones are already sorted by address
block_t	*_next_zone(bool reset)
{
	static block_t	*block = NULL;
	int				zone;

	if (reset || !block)
	{
		block = *first_block();
		return (block);
	}
	zone = block->_zone;
	for ( ; block && block->_next; block = block->_next)
	{
		if (DIFF((size_t)block, (size_t)block->_next) != block->_size)
		{
			block = block->_next;
			return (block);
		}
		if (zone != block->_zone)
			return (block);
		if (BLOCK_SIZE == block->_size) // TODO: check if it's the zone's last block
		{
			block = block->_next;
			return (block);
		}
	}
	return (NULL);
}

static void	_show_alloc_mem_internal(void)
{
	static const char	*_zone_name[] = {
		"TINY ",
		"SMALL",
		"LARGE"
	};

	block_t				*zone = _next_zone(true);
	block_t				*next_zone = NULL;
	block_t				*block = NULL;

	for ( ; zone; zone = next_zone)
	{
		next_zone = _next_zone(false);
		
		ft_putstr(_zone_name[zone->_zone]);
		ft_putstr(": ");
		ft_putaddr(zone, 0);
		ft_putstr("\n");

		for (block = zone; block && block != next_zone; block = block->_next)
		{
			ft_putstr("  ");
			ft_putaddr(block, 0);
			ft_putstr(" - ");
			ft_putaddr((void *)block + block->_size, 0);
			ft_putstr(" : ");
			ft_putnbr(block->_size, 0);
			ft_putstr(" bytes\n");
		}
	}
}

void	show_alloc_mem(void)
{
	MLOG("show_alloc_mem()");

	pthread_mutex_lock(&g_malloc_mutex);
	_show_alloc_mem_internal();
	pthread_mutex_unlock(&g_malloc_mutex);
}
