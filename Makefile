# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/25 01:10:15 by besellem          #+#    #+#              #
#    Updated: 2022/03/21 15:57:09 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		:=	libft_malloc_$(HOSTTYPE).so
NAME_LINK	:=	libft_malloc.so
EXECUTABLE	:=	malloc_test

## Main directories
BUILD_DIR	:=	.build
SRCS_DIR	:=	srcs
INCS		:=	-I incs
SUB_DIR		:=	srcs
OBJ_DIR 	:=	$(BUILD_DIR)/obj
DIRS		:=	$(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_DIR))


## Main srcs
SRCS	:=	calloc.c \
			free.c \
			malloc.c \
			realloc.c \
			reallocf.c \
			show_alloc_mem.c \
			show_memory_leaks.c \
			utils.c

## Objects
OBJS 		:=	$(SRCS:%.c=$(OBJ_DIR)/%.o)


## Commands
CC			:=	clang
CFLAGS		:=	-Wall -Wextra -Werror# -g3 -fsanitize=address #-O1
LIBC		:=	ar rc
RM			:=	rm -f


# Colors
CLR			:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m


## Compilation
$(NAME):	$(OBJS)
			@echo "Creating $(RED)$@ $(CLR)..."
			@$(LIBC) $(NAME) $(OBJS)
			@ln -s $(NAME) $(NAME_LINK) 2>/dev/null

			@$(CC) $(CFLAGS) $(INCS) main.c $(NAME_LINK) -o $(EXECUTABLE)

			@echo "$(GREEN)Compilation $(YELLOW)of $(RED)$@ $(BLUE)done$(CLR)"

all:		$(NAME)

bonus:		all

clean:
			@ echo "Deleting $(CYAN)malloc $(CLR)objs ..."
			@ $(RM) $(OBJS)
			@ rm -rf $(BUILD_DIR)
			@ $(RM) -rf $(EXECUTABLE) $(EXECUTABLE).dSYM

fclean:		clean
			@ echo "Deleting $(CYAN)malloc $(CLR)library ..."
			@ $(RM) $(NAME) $(NAME_LINK)

re:			fclean all

$(BUILD_DIR):
			@ echo "Creating $(RED)$@ $(CLR)..."
			@ mkdir $@ $(DIRS)

$(OBJ_DIR)/%.o:$(SRCS_DIR)/%.c ./incs/malloc.h | $(BUILD_DIR)
			@ echo "Compiling $(YELLOW)$< $(CLR)..."
			@ $(CC) $(CFLAGS) -c $< -o $@ $(INCS)

.PHONY:		all clean fclean re
