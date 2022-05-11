/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:25 by besellem          #+#    #+#             */
/*   Updated: 2022/05/11 17:54:17 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H


/*
** -- MALLOC ENV MACROS --
*/
#if 1
# define MALLOC_DEBUG
#endif

#if 0
# define MALLOC_VERBOSE
#endif


/*
** -- INCLUDES --
*/
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <stddef.h>
#include <stdbool.h>
#include "defs.h"


/*
** -- GLOBALS --
*/
extern pthread_mutex_t	g_malloc_mutex;


/*
** -- DEFINES --
*/
#define RED          "\033[1;31m"
#define GREEN        "\033[1;32m"
#define CYAN         "\033[1;36m"
#define BLUE         "\033[1;34m"
#define CLR          "\033[0m"

#define BLOCK_FREED  0
#define BLOCK_IN_USE 1
#define BLOCK_SIZE   sizeof(block_t)

#define ZONE_TINY    ((size_t)(getpagesize() *  4))
#define ZONE_SMALL   ((size_t)(getpagesize() * 32))

#define TINY         ( ZONE_TINY / 128UL)
#define SMALL        (ZONE_SMALL / 128UL)


/*
** -- LOGS & DEBUGS --
*/
#define LOG(__msg)                                                             \
	ft_putstr(CYAN);                                                           \
	ft_putstr(__FILE__);                                                       \
	ft_putstr(":");                                                            \
	ft_putnbr(__LINE__, 0);                                                    \
	ft_putstr(": " CLR);                                                       \
	ft_putstr(((__msg) != NULL) ? (__msg) : "Here");                           \
	ft_putstr("\n");

#ifdef MALLOC_VERBOSE
# define MLOG(__func_name) LOG(__func_name)
#else
# define MLOG(__func_name)
#endif

#define print_blocks() LOG(NULL); _print_blocks();


/*
** -- FUNCTION-LIKE MACROS --
*/

/*
** Find which zone size is best fitted to allocate.
** We add BLOCK_SIZE to __size to be sure that the zone size will always be
** greater than the size requested.
*/
#define define_block_size(__size)                                              \
	(((__size) + BLOCK_SIZE <= ZONE_TINY) ? ZONE_TINY :                        \
		(((__size) + BLOCK_SIZE <= ZONE_SMALL) ? ZONE_SMALL :                  \
			(__size + BLOCK_SIZE)))


/*
** find the next aligned address / size.
*/
#if   4 == __SIZEOF_SIZE_T__ // 32 bits
# define align(__x) ((((__x) - 1) >> 2) << 2) + __SIZEOF_SIZE_T__
#elif 8 == __SIZEOF_SIZE_T__ // 64 bits
# define align(__x) ((((__x) - 1) >> 3) << 3) + __SIZEOF_SIZE_T__
#else
# error "Unsupported architecture: what kind of system do you own ?"
#endif


#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#define DIFF(x, y) (MAX(x, y) - MIN(x, y))


/*
** need two blocks: one for the actual data and the second for the footer
** (is the bond between this zone and the next one)
** (BLOCK_SIZE << 1) is actually an optimized version of (BLOCK_SIZE * 2)
*/
#define get_sanitized_size(__size)  align((__size) + (BLOCK_SIZE << 1))

#define get_ptr_meta(__ptr)         ((block_t *)((void *)(__ptr) - BLOCK_SIZE))
#define get_ptr_user(__ptr)         ((void *)(__ptr) + BLOCK_SIZE)


/*
** -- DATA STRUCTURES & TYPES --
*/

// TODO: unused, to remove
typedef	struct s_debug_data	t_debug_data;
struct s_debug_data
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
	block_t			*_next;
};


/*
** -- PROTOTYPES --
*/
void		*ft_memset(void *b, int c, size_t len) NOEXPORT;
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		ft_putaddr(const void *addr, int pad);
void		ft_putaddr_fd(const void *addr, int fd, int pad);
void		ft_putstr(const char *s);
void		ft_putstr_fd(int fd, const char *s);
void		ft_putnstr(char *s, size_t n);
void		ft_putnbr(size_t n, int pad);
void		ft_putnbr_fd(int fd, size_t n, int pad);
int			ft_nblen_base(size_t n, int base);
void		_print_blocks(void);

block_t		**first_block(void);
block_t		*last_block(void);
block_t		*_next_zone(bool reset);
void		split_block(block_t *block, size_t size);

/* join all contiguous freed blocks on each zones */
void		defragment_blocks(void);

void		_free_internal(void *ptr);
void		*_malloc_internal(size_t size);

#endif
