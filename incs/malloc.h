/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 09:57:55 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 14:24:28 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <stddef.h>


/*
** malloc() allocates `size' bytes of memory.
*/
void	*malloc(size_t size);


/*
** realloc() tries to change the size of the memory block pointed to by `ptr'
*/
void	*realloc(void *ptr, size_t size);


/*
** reallocf() behave like realloc() except that it frees the original memory if
** the reallocation fails.
** It's a FreeBSD extension.
*/
void	*reallocf(void *ptr, size_t size);


/*
** calloc() behave like malloc() except that it sets each bytes of the allocated
** memory to 0.
*/
void	*calloc(size_t count, size_t size);


/*
** free() deallocates the memory space pointed to by ptr, which must have been a
** pointer returned by a previous call to malloc(), calloc(), or realloc().
*/
void	free(void *ptr);


void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);
void	show_memory_leaks(void);

#endif
