/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2021/11/03 00:10:39 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

/*
** -- INCLUDES --
*/
# include <stdio.h> // to remove
# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stddef.h>


/*
** -- DEFINES --
*/
# define RED   "\e[1;31m"
# define GREEN "\e[1;32m"
# define CYAN  "\e[1;36m"
# define BLUE  "\e[1;34m"
# define CLR   "\e[0m"

# define MALLOC_DEBUG
// # define THREAD_SAFE

# define BLOCK_FREED  0
# define BLOCK_IN_USE 1
# define BLOCK_SIZE   sizeof(block_t)

# define TINY          64UL
# define SMALL        512UL

# define ZONE_TINY    align(TINY  * 100)
# define ZONE_SMALL   align(SMALL * 100)

# if defined(MALLOC_DEBUG)
#  define LOG          printf(GREEN "%s:%d:" CLR " Here\n", __FILE__, __LINE__);
# else
#  define LOG          ;
# endif /* defined(MALLOC_DEBUG) */


/*
** -- FUNCTION-LIKE MACROS --
*/

/* define which zone size is best fitted to allocate */
# define define_block_size(size) \
	(((size) <= TINY) ? ZONE_TINY : (((size) <= SMALL) ? ZONE_SMALL : (size)));


/* align() will help us aligning memory */
# if   __SIZEOF_SIZE_T__ == 4 // 32 bits
#  define align(x) ((((x) - 1) >> 2) << 2) + __SIZEOF_SIZE_T__
# elif __SIZEOF_SIZE_T__ == 8 // 64 bits
#  define align(x) ((((x) - 1) >> 3) << 3) + __SIZEOF_SIZE_T__
# else
#  error "Unsupported architecture: what kind of system do you own ??"
# endif


/*
** -- DATA STRUCTURES --
*/
// typedef struct	s_malloc
// {
// 	void				*_ptr;
// 	pthread_mutex_t		_m;
// }	malloc_t;


typedef unsigned long long		wide_int_t;
typedef struct s_block			block_t;

// struct __attribute__((packed)) s_block
struct s_block
{
	unsigned char	_in_use;
	size_t			_size;
	block_t			*_next;
};



/*
** -- PROTOTYPES --
*/
void	*ft_print_memory(void *addr, unsigned int size);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);

block_t		**first_block(void);
block_t		**last_block(void);

#endif
