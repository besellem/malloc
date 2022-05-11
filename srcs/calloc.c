/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:45 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 08:16:18 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;
	
	MLOG("calloc()");
	
	/* allocate minimum sized object */
	if (0 == count || 0 == size)
		count = size = 1;

	ptr = malloc(count * size);
	if (ptr)
		ft_memset(ptr, 0, count * size);
	return (ptr);
}
