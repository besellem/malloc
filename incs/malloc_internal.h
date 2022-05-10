/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2022/05/10 17:15:26 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# if 1
#  define MALLOC_DEBUG
# endif

/*
** -- INCLUDES --
*/
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

# define SYSCALL_ERR  (-1)

# define BLOCK_FREED  0
# define BLOCK_IN_USE 1
# define BLOCK_SIZE   sizeof(block_t)

# define ZONE_TINY    ((size_t)(getpagesize() *  4))
# define ZONE_SMALL   ((size_t)(getpagesize() * 32))

# define TINY         ( ZONE_TINY / 128UL)
# define SMALL        (ZONE_SMALL / 128UL)


# define LOG(msg)  \
	ft_putstr(CYAN); \
	ft_putstr(__FILE__); \
	ft_putstr(":"); \
	ft_putnbr(__LINE__, 0); \
	ft_putstr(": " CLR); \
	ft_putstr((msg != NULL) ? msg : "Here"); \
	ft_putstr("\n");


# define print_blocks() LOG(NULL); _print_blocks();

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


/* align() will help us aligning memory */
# if   __SIZEOF_SIZE_T__ == 4 // 32 bits
#  define align(x) ((((x) - 1) >> 2) << 2) + __SIZEOF_SIZE_T__
# elif __SIZEOF_SIZE_T__ == 8 // 64 bits
#  define align(x) ((((x) - 1) >> 3) << 3) + __SIZEOF_SIZE_T__
# else
#  error "Unsupported architecture: what kind of system do you own ?"
# endif

# define MIN(x, y)       ((x) < (y) ? (x) : (y))
# define MAX(x, y)       ((x) > (y) ? (x) : (y))
# define DIFF(x, y)      (MAX(x, y) - MIN(x, y))


/*
** need two blocks: one for the actual data and the second for the footer
** (is the bond between this zone and the next one)
** (BLOCK_SIZE << 1) is actually an optimized version of (BLOCK_SIZE * 2)
*/
# define get_sanitized_size(__size)  align(__size + (BLOCK_SIZE << 1))

# define get_ptr_meta(__ptr)         ((block_t *)((void *)(__ptr) - BLOCK_SIZE))
# define get_ptr_user(__ptr)         ((void *)(__ptr) + BLOCK_SIZE)


/*
** -- DATA STRUCTURES & TYPES --
*/

/* Biggest integer size (128 bits or lower) */
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
typedef	struct s_block	block_t;
struct s_block
{
	unsigned char	_zone : 2;
	unsigned char	_status : 2;
	size_t			_size;
	block_t	*_next;
};


/*
** -- PROTOTYPES --
*/
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		ft_putaddr(const void *addr, int pad);
void		ft_putaddr_fd(const void *addr, int fd, int pad);
void		ft_putstr(const char *s);
void		ft_putstr_fd(int fd, const char *s);
void		ft_putnstr(char *s, size_t n);
void		ft_putnbr(long long n, int pad);
void		ft_putnbr_fd(int fd, long long n, int pad);
int			ft_nblen_base(long long n, int base);

block_t		**first_block(void);
block_t		*last_block(void);
block_t		*_next_zone(bool reset);
void		_print_blocks(void);
void		split_block(block_t *block, size_t size);

/* join all contiguous freed blocks on each zones */
void		defragment_blocks(void);

void		deallocate_empty_zones(void);
void		_free_all_blocks(void);

/*
** a wrapper of free() to print debug info.
** works with MALLOC_DEBUG macro
*/
void		__free(void *ptr, const struct s_debug_data debug);

#endif
