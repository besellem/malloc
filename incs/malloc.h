/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 09:57:55 by besellem          #+#    #+#             */
/*   Updated: 2022/03/09 13:47:15 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "malloc_internal.h"

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*reallocf(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);

void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);
void	show_memory_leaks(void);

#endif
