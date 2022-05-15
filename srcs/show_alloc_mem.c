/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:46:32 by besellem          #+#    #+#             */
/*   Updated: 2022/05/15 13:23:45 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

static void	_show_alloc_mem_internal(void)
{
	static const char	*_zone_name[] = {
		"TINY ",
		"SMALL",
		"LARGE"
	};

	block_t				*zone = get_next_zone(true);
	block_t				*next_zone = NULL;
	block_t				*block = NULL;
	size_t				total_size = 0;

	for ( ; zone; zone = next_zone)
	{
		next_zone = get_next_zone(false);
		
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

			total_size += block->_size;
		}
	}
	ft_putstr("Total : ");
	ft_putnbr(total_size, 0);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem(void)
{
	MLOG("show_alloc_mem()");

	pthread_mutex_lock(&g_malloc_mutex);
	_show_alloc_mem_internal();
	pthread_mutex_unlock(&g_malloc_mutex);
}
