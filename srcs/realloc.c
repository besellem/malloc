/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:49 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:07:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

// static void	_init(malloc_t *p)
// {
// 	ft_memset(p, 0, sizeof(malloc_t));
// #ifdef THREAD_SAFE
// 	pthread_mutex_init(&p->_m, NULL);
// 	pthread_mutex_lock(&p->_m);
// #endif /* THREAD_SAFE */
// }

// static void	_end(malloc_t *p)
// {
// #ifdef THREAD_SAFE
// 	pthread_mutex_unlock(&p->_m);
// 	pthread_mutex_destroy(&p->_m);
// #endif /* THREAD_SAFE */
// 	ft_memset(p, 0, sizeof(malloc_t));
// }

void	*realloc(__unused void *ptr, __unused size_t size)
{
	void	*new_ptr = NULL;
	size_t	ptr_size = 0;

	if (NULL == ptr)
		return (malloc(size));
	ptr_size = get_ptr_size((void *)ptr - BLOCK_SIZE);
	new_ptr = malloc(size + ptr_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, ptr_size);
	return (new_ptr);
}
