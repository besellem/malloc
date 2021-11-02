/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:41 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:09:52 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

// void	*malloc(size_t size)
// {
// 	return (realloc(NULL, size));
// }


block_t		**first_block(void)
{
	static block_t	*block = NULL;

	return (&block);
}

block_t		**last_block(void)
{
	static block_t	*block = NULL;

	return (&block);
}


//         0  1
// 128 -> 64 64
// +20 + 4

//  1  0  1
// 24 40 64


void		split_block(block_t *block, size_t size)
{
	const size_t	old_size = block->_size;
	block_t			*old_block = block->_next;

	block->_in_use = BLOCK_IN_USE;
	block->_size = size;
	block->_next = (void *)block + size;
	block->_next->_in_use = BLOCK_FREED;
	block->_next->_size = old_size - size;
	block->_next->_next = old_block;
}

void		add_block(block_t *block)
{
	block_t	*last = *last_block();

	if (NULL == *first_block())
		*first_block() = block;
	if (last)
	{
		last->_next = block;
	}
	*last_block() = block->_next;
}

block_t		*find_block(size_t size)
{
	block_t	*block = *first_block();

	while (block)
	{
		if (BLOCK_FREED == block->_in_use && block->_size >= size)
		{
			split_block(block, size);
			
			// block_t	*last = *last_block();
			// if (last)
			// {
			// 	last->_next = block->_next;
			// }
			// *last_block() = block->_next;

			return (block);
		}
		block = block->_next;
	}
	return (NULL);
}

void		print_blocks(void)
{
	block_t	*block = *first_block();

	printf(GREEN "\n.................BLOCKS................." CLR "\n");
	printf(CYAN "    addr    |    next    |    size    | used" CLR "\n");
	while (block)
	{
		printf("%11p:  %11p  %10zu    %u\n",
			block, block->_next, block->_size, block->_in_use);
		block = block->_next;
	}
	printf(GREEN "...............BLOCKS.END..............." CLR "\n\n");
}

block_t		*create_block(size_t size)
{
	const size_t	_siz = define_block_size(size);
	block_t			*block = mmap(NULL, _siz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	printf(BLUE "Warning:" CLR " mmap(%zu)\n", _siz);

	if (MAP_FAILED == block)
	{

#ifndef MALLOC_DEBUG
		
		dprintf(STDERR_FILENO, RED "Error:" CLR " mmap(%zu)\n", _siz);
		
#endif /* MALLOC_DEBUG */

		return (NULL);
	}
	block->_size = _siz;
	block->_next = NULL;
	split_block(block, size);

	LOG;
	print_blocks();
	printf("before -> first[%p] last[%p]\n", *first_block(), *last_block());

	add_block(block);

	printf("after  -> first[%p] last[%p]\n", *first_block(), *last_block());
	LOG;
	print_blocks();
	
	return (block);
}

void	*malloc(size_t size)
{
	const size_t	_size = align(size + BLOCK_SIZE);
	block_t			*block;

	printf(GREEN ">" CLR " IN MALLOC\n");

	if (0 == size)
		return (NULL);
	block = find_block(_size);
	if (!block)
	{
		block = create_block(_size);
		if (!block)
			return (NULL);
	}
	
	// ft_print_memory(block, size + BLOCK_SIZE);
	printf("in_use[%d] size[%zu]\n", block->_in_use, block->_size);
	print_blocks();
	
	printf(GREEN "<" CLR " IN MALLOC\n");
	return ((void *)block + BLOCK_SIZE);
}
