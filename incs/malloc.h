/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 09:57:55 by besellem          #+#    #+#             */
/*   Updated: 2021/10/18 10:50:38 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "malloc_internal.h"

# define TINY  128
# define SMALL 1024
# define LARGE 4096

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*reallocf(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);

void	show_alloc_mem(void);

typedef struct	s_malloc
{
	void				*_ptr;
	pthread_mutex_t		_m;
}	malloc_t;

#endif
