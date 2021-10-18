/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2021/10/18 10:26:31 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
// # include <stddef.h>

void	*ft_memset(void *b, int c, size_t len);

#endif
