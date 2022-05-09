/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_memory_leaks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:44:34 by besellem          #+#    #+#             */
/*   Updated: 2022/05/09 15:41:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"

static void	_free_all_blocks(void)
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

static void	_show_memory_leaks_wrapper(bool free_all)
{
	const block_t	*block = *first_block();
	size_t			sanitized_leaks = 0;
	size_t			real_leaks = 0;

	for ( ; block != NULL; block = block->_next)
	{
		if (BLOCK_IN_USE == block->_status)
		{
			ft_putstr(RED "* mem leak" CLR " -> ");
			ft_putaddr(block, 11);
			ft_putstr(" (");
			ft_putnbr(block->_size - BLOCK_SIZE, 0);
			ft_putstr(" bytes)\n");
			sanitized_leaks += block->_size - BLOCK_SIZE;
		}
		real_leaks += block->_size;
	}

	if (free_all)
		_free_all_blocks();

	if (0 == real_leaks)
		ft_putstr(GREEN "*** No memory leaks ***" CLR "\n");
	else
	{
		const int _padding = MAX(ft_nblen_base(real_leaks, 10), ft_nblen_base(sanitized_leaks, 10));
		
		ft_putstr(RED "  Total memory leaked" CLR "  -> ");
		ft_putnbr(sanitized_leaks, _padding);
		ft_putstr(" bytes\n");
		
		ft_putstr(RED "  Total used by mmap()" CLR " -> ");
		ft_putnbr(real_leaks, _padding);
		ft_putstr(" bytes\n");
	}
}

void	show_memory_leaks(bool free_all)
{
	pthread_mutex_t		_m;

	pthread_mutex_init(&_m, NULL);
	pthread_mutex_lock(&_m);

	_show_memory_leaks_wrapper(free_all);

	pthread_mutex_unlock(&_m);
	pthread_mutex_destroy(&_m);
}
