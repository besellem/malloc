/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:32:39 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 09:38:51 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_internal.h"

void	*reallocf(void *ptr, size_t size)
{
	MLOG("reallocf()");

	void	*p = realloc(ptr, size);

	if (NULL == p)
		free(ptr);
	return (p);
}
