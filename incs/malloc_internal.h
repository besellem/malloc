/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2022/03/30 21:44:40 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

/*
** -- INCLUDES --
*/
# include <stdio.h>  // to remove
# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stddef.h>
# include <stdbool.h>


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

# define SYSCALL_ERR  (-1)

# define BLOCK_FREED  0
# define BLOCK_IN_USE 1
# define BLOCK_SIZE   sizeof(block_t)

# define ZONE_TINY    ((size_t)(getpagesize() *  4))
# define ZONE_SMALL   ((size_t)(getpagesize() * 32))

# define TINY         ( ZONE_TINY / 128UL)
# define SMALL        (ZONE_SMALL / 128UL)

# define MIN(x, y)    ((x) < (y) ? (x) : (y))
# define MAX(x, y)    ((x) > (y) ? (x) : (y))
# define DIFF(x, y)   (MAX(x, y) - MIN(x, y))


// TO REMOVE
# if defined(MALLOC_DEBUG)
#  define LOG             printf(BLUE "%s:%d:" CLR " Here\n", __FILE__, __LINE__);
#  define print_blocks()  LOG; _print_blocks_wrapper()
# else
#  define LOG
#  define print_blocks()
# endif /* defined(MALLOC_DEBUG) */


/*
** -- FUNCTION-LIKE MACROS --
*/

/*
** Define which zone size is best fitted to allocate.
** We add BLOCK_SIZE to the size to be sure that the zone size will always be
** greater than the size requested.
*/
# define define_block_size(__size) \
	(((__size) + BLOCK_SIZE <= ZONE_TINY) ? ZONE_TINY : \
		(((__size) + BLOCK_SIZE <= ZONE_SMALL) ? ZONE_SMALL : \
			(__size + BLOCK_SIZE)))


/*
** need two blocks: one for the actual data and the second for the footer
** (is the bond between this zone and the next one)
** (BLOCK_SIZE << 1) is actually an optimized version of (BLOCK_SIZE * 2)
*/
# define get_sanitized_size(__size)  align(__size + (BLOCK_SIZE << 1))

# define get_ptr_meta(__ptr)         ((block_t *)((void *)(__ptr) - BLOCK_SIZE))
# define get_ptr_user(__ptr)         ((void *)(__ptr) + BLOCK_SIZE)


/* align() will help us aligning memory */
# if   __SIZEOF_SIZE_T__ == 4 // 32 bits
#  define align(x) ((((x) - 1) >> 2) << 2) + __SIZEOF_SIZE_T__
# elif __SIZEOF_SIZE_T__ == 8 // 64 bits
#  define align(x) ((((x) - 1) >> 3) << 3) + __SIZEOF_SIZE_T__
# else
#  error "Unsupported architecture: what kind of system do you own ?"
# endif


/*
** -- DATA STRUCTURES & TYPES --
*/

/* Bigger integer size (128 bits or other) */
# ifdef __SIZEOF_INT128__
typedef __uint128_t            wide_int_t;
# else
typedef unsigned long long     wide_int_t;
# endif


struct	s_debug_data
{
	void	*ptr;
	char	*ptr_name;
	char	*file;
	int		line;
};


enum
{
	MASK_ZONE_TINY,
	MASK_ZONE_SMALL,
	MASK_ZONE_LARGE
};


/*
** block list containing all allocated blocks:
**  _zone   : zone type of the block (uses MASK_ZONE_* constants)
**  _status : `BLOCK_IN_USE' if block is in use, `BLOCK_FREED' if free
**  _size   : size of the block (also contains meta size)
**  _next   : pointer to the next block in the list
*/
typedef	struct // __attribute__((packed)) // error maker
s_block
{
	unsigned char	_zone : 2;
	unsigned char	_status : 2;
	size_t			_size;
	struct s_block	*_next;
}	block_t;


/*
** -- PROTOTYPES --
*/
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t n);

block_t		**first_block(void);
block_t		*last_block(void);
void		_print_blocks_wrapper(void);
void		split_block(block_t *block, size_t size);

/* join all contiguous freed blocks */
void		join_blocks(void);


/*
** a wrapper of free() to print debug info
** working with MALLOC_DEBUG macro
*/
void		__free(void *ptr, const struct s_debug_data debug);

#endif
