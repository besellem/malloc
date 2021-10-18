/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:49 by besellem          #+#    #+#             */
/*   Updated: 2021/10/18 10:56:11 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/malloc.h"

static void	_init(malloc_t *p)
{
	ft_memset(p, 0, sizeof(malloc_t));
	pthread_mutex_init(&p->_m, NULL);
	pthread_mutex_lock(&p->_m);
}

static void	_end(malloc_t *p)
{
	pthread_mutex_unlock(&p->_m);
	pthread_mutex_destroy(&p->_m);
	ft_memset(p, 0, sizeof(malloc_t));
}

void	*realloc(void *ptr, size_t size)
{
	void		*new_ptr;
	malloc_t	_p;

	_init(&_p);
	new_ptr = _p._ptr;
	_end(&_p);
	return (new_ptr);
}