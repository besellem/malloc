/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2021/11/02 01:28:31 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <stdio.h> // to remove
# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stddef.h>

# define RED   "\e[1;31m"
# define GREEN "\e[1;32m"
# define BLUE  "\e[1;34m"
# define CLR   "\e[0m"

# define MALLOC_DEBUG
// # define THREAD_SAFE

# define BLOCK_FREED  0
# define BLOCK_IN_USE 1

# define TINY         32
# define SMALL        128
# define LARGE        1024

# define ZONE_TINY    (TINY * 100)
# define ZONE_SMALL   (SMALL * 100)
# define ZONE_LARGE   (LARGE * 100)


typedef struct	s_malloc
{
	void				*_ptr;
	pthread_mutex_t		_m;
}	malloc_t;

// struct __attribute__((packed)) s_block
struct s_block
{
	unsigned char	_in_use;
	size_t			_size;
	// size_t			_tail_size;
};
typedef struct s_block		block_t;


void	*ft_print_memory(void *addr, unsigned int size);
void	*ft_memset(void *b, int c, size_t len);

#endif
