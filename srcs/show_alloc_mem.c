/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:46:32 by besellem          #+#    #+#             */
/*   Updated: 2022/05/10 16:19:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"
#include <stdbool.h>

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
	}
	return (NULL);
}

// static
block_t	*_next_sorted_zone(bool reset)
{
	static block_t	*last_min = NULL;
	static block_t	*max = NULL;
	block_t			*zone = _next_zone(true);
	block_t			*min;

	if (reset)
	{
		last_min = zone;
		max = NULL;
		for ( ; zone; zone = _next_zone(false))
		{
			if (last_min > zone)
				last_min = zone;
			if (max < zone)
				max = zone;
		}
		return (last_min);
	}
	if (zone == last_min)
		zone = _next_zone(false);
	min = max;
	for ( ; zone; zone = _next_zone(false))
	{
		if (min > last_min)
		{
			if (min > zone && zone > last_min)
				min = zone;
		}
	}
	if (last_min == max)
		return (NULL);
	last_min = min;
	return (last_min);
}

static void	_show_alloc_mem_wrapper(void)
{
	static const char	*_zone_name[] = {
		"TINY ",
		"SMALL",
		"LARGE"
	};

#ifdef __linux__
	block_t				*zone = _next_zone(true);
#else
	block_t				*zone = _next_sorted_zone(true);
#endif

	block_t				*next_zone = NULL;
	block_t				*block = NULL;

	for ( ; zone; zone = next_zone)
	{
#ifdef __linux__
		next_zone = _next_zone(false);
#else
		next_zone = _next_sorted_zone(false);
#endif
		
		ft_putstr(_zone_name[zone->_zone]);
		ft_putstr(": ");
		ft_putaddr(zone, 0);
		ft_putstr("\n");
		
		for (block = zone; block && block != next_zone; block = block->_next)
		{
			ft_putstr("  ");
			ft_putaddr(block, 0);
			ft_putstr(" - ");
			ft_putaddr(block + block->_size, 0);
			ft_putstr(" : ");
			ft_putnbr(block->_size, 0);
			ft_putstr(" bytes\n");
		}
	}
}

void	show_alloc_mem(void)
{
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	_show_alloc_mem_wrapper();

	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);
}
