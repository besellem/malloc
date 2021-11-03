# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/25 01:10:15 by besellem          #+#    #+#              #
#    Updated: 2021/11/03 21:36:46 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		:=	libft_malloc_$(HOSTTYPE).so
NAME_LINK	:=	libft_malloc.so

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
			utils.c

## Objects
OBJS 		:=	$(SRCS:%.c=$(OBJ_DIR)/%.o)


## Commands
CC			:=	clang
CFLAGS		:=	-Wall -Wextra -Werror# -g3 -fsanitize=address #-O1
LIBC		:=	ar rc
RM			:=	rm -f


# Colors
CLR_COLOR	:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m


## Compilation
$(NAME):	$(OBJS) ./main.c
			@echo "Creating $(RED_COLOR)$@ $(CLR_COLOR)..."
			@$(LIBC) $(NAME) $(OBJS)
			@ln -s $(NAME) $(NAME_LINK)

			@$(CC) $(CFLAGS) $(INCS) main.c $(NAME_LINK)

			@echo "$(GREEN)Compilation $(YELLOW)of $(RED)$@ $(BLUE)done$(CLR_COLOR)"

all:		$(NAME)

bonus:		all

clean:
			@ echo "Deleting $(CYAN)malloc $(CLR_COLOR)objs ..."
			@ $(RM) $(OBJS)
			@ rm -rf $(BUILD_DIR)
			@ $(RM) -rf a.out a.out.dSYM

fclean:		clean
			@ echo "Deleting $(CYAN)malloc $(CLR_COLOR)library ..."
			@ $(RM) $(NAME) $(NAME_LINK)

re:			fclean all

$(BUILD_DIR):
			@ echo "Creating $(RED)$@ $(CLR_COLOR)..."
			@ mkdir $@ $(DIRS)

$(OBJ_DIR)/%.o:$(SRCS_DIR)/%.c ./incs/malloc.h | $(BUILD_DIR)
			@ echo "Compiling $(YELLOW)$< $(CLR_COLOR)..."
			@ $(CC) $(CFLAGS) -c $< -o $@ $(INCS)

.PHONY:		all clean fclean re
