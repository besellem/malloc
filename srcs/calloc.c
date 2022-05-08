/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:02:45 by besellem          #+#    #+#             */
/*   Updated: 2022/05/08 15:33:07 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"
#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr = malloc(count * size);

	if (ptr)
		ft_memset(ptr, 0, count * size);
	return (ptr);
}
