/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2021/11/05 00:58:05 by besellem         ###   ########.fr       */
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

# if 1
#  define MALLOC_DEBUG
# endif
/* # define THREAD_SAFE */

# define SYSCALL_ERR  (-1)

# define BLOCK_FREED  0
# define BLOCK_IN_USE 1
# define BLOCK_SIZE   sizeof(block_t)

# define TINY         ( 32UL + BLOCK_SIZE)
# define SMALL        (256UL + BLOCK_SIZE)

# define ZONE_TINY    align( TINY * 100)
# define ZONE_SMALL   align(SMALL * 100)


# define MIN(x, y)    ((x) < (y) ? (x) : (y))
# define MAX(x, y)    ((x) > (y) ? (x) : (y))


# if defined(MALLOC_DEBUG)
#  define LOG         printf(GREEN "%s:%d:" CLR " Here\n", __FILE__, __LINE__);
# else
#  define LOG
# endif /* defined(MALLOC_DEBUG) */


/*
** -- FUNCTION-LIKE MACROS --
*/

/* define which zone size is best fitted to allocate */
# define define_block_size(size) \
	(((size) <= TINY) ? ZONE_TINY : (((size) <= SMALL) ? ZONE_SMALL : (size)));


# define get_ptr_meta(ptr) ((block_t *)((void *)ptr - BLOCK_SIZE))
# define get_ptr_user(ptr) ((void *)ptr + BLOCK_SIZE)


/* align() will help us aligning memory */
# if   __SIZEOF_SIZE_T__ == 4 // 32 bits
#  define align(x) ((((x) - 1) >> 2) << 2) + __SIZEOF_SIZE_T__
# elif __SIZEOF_SIZE_T__ == 8 // 64 bits
#  define align(x) ((((x) - 1) >> 3) << 3) + __SIZEOF_SIZE_T__
# else
#  error "Unsupported architecture: what kind of system do you own ?"
# endif


/*
** -- DATA STRUCTURES --
*/
// typedef struct	s_malloc
// {
// 	void				*_ptr;
// 	pthread_mutex_t		_m;
// }	malloc_t;


/* Bigger integer size (128 bits or other) */
# ifdef __SIZEOF_INT128__
typedef __uint128_t            wide_int_t;
# else
typedef unsigned long long     wide_int_t;
# endif


/*
** block list containing all allocated blocks:
**  _status : `BLOCK_IN_USE' if block is in use, `BLOCK_FREED' if free
**  _size   : size of the block (also contains meta size)
**  _next   : pointer to the next block in the list
*/
typedef struct //__attribute__((packed))
s_block
{
	unsigned char	_status : 2;
	size_t			_size;
	struct s_block	*_next;
}	block_t;


/*
** -- PROTOTYPES --
*/
void	*ft_print_memory(void *addr, unsigned int size);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);

block_t		**first_block(void);
block_t		**last_block(void);
void		print_blocks(void);
void		split_block(block_t *block, size_t size);

#endif
