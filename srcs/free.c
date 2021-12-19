/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:21 by besellem          #+#    #+#             */
/*   Updated: 2021/11/02 19:40:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"


void	ft_check_leaks(void)
{
	block_t	*block = *first_block();
	size_t	sanitized_leaks = 0;
	size_t	real_leaks = 0;

	for ( ; block != NULL; block = block->_next)
	{
		if (BLOCK_IN_USE == block->_in_use)
		{
			sanitized_leaks += block->_size - BLOCK_SIZE;
			real_leaks += block->_size;
			printf(RED "leak" CLR " -> addr[%11p] size[%zu]\n",
				block, block->_size - BLOCK_SIZE);
		}
	}
	printf(RED "total leaked" CLR "         -> %zu bytes\n", sanitized_leaks);
	printf(RED "total used by mmap()" CLR " -> %zu bytes\n", real_leaks);
}


/* joins two blocks starting with `block' */
void	join_blocks(block_t *block)
{
	block_t	*_next;

	if (!block || !block->_next)
		return ;

	_next = block->_next;
	if (BLOCK_FREED == block->_in_use && BLOCK_FREED == _next->_in_use)
	{
		block->_size += _next->_size;
		block->_next = _next->_next;
	}
}

void	free(void *ptr)
{
	block_t	*block;
	block_t	*tmp;

	printf(GREEN ">" CLR " IN FREE\n");
	
	if (NULL == ptr)
		return ;

	/* there's no allocated blocks left, must be a double free */
	if (!*first_block())
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n", ptr);
		return ;
	}
	
	block = (block_t *)get_ptr_meta(ptr);
	if (BLOCK_FREED == block->_in_use)
	{
		dprintf(STDERR_FILENO, BLUE "Warning:" CLR " Attempting double free on address %p\n", ptr);
	}
	else
	{
		block->_in_use = BLOCK_FREED;
		tmp = *first_block();
		while (tmp->_next)
		{
			if (BLOCK_FREED == tmp->_in_use && BLOCK_FREED == tmp->_next->_in_use)
			{
				join_blocks(tmp);
				tmp = *first_block();
			}
			else
				tmp = tmp->_next;
		}
			
		if (*first_block() && NULL == (*first_block())->_next)
		{

#ifndef MALLOC_DEBUG

			munmap(*first_block(), (*first_block())->_size);

#else /* MALLOC_DEBUG */

			if (SYSCALL_ERR == munmap(*first_block(), (*first_block())->_size))
			{
				dprintf(STDERR_FILENO, RED "Error: " CLR "munmap(%p, %zu)\n",
					*first_block(), (*first_block())->_size);
			}
			else
			{
				printf(CYAN "%p" CLR ": pointer freed\n", ptr);
			}
			*first_block() = NULL;
		}

#endif /* !MALLOC_DEBUG */

	}
	printf(GREEN "<" CLR " IN FREE\n");
}
