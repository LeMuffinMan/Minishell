# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/01 17:29:24 by oelleaum          #+#    #+#              #
#    Updated: 2025/04/01 17:44:57 by oelleaum         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
INC = -I include
INC_LIBFT = -I libft/include

SRC_DIR = src
OBJ_DIR = obj

LIBFT_SRC_DIR = libft/src
LIBFT_OBJ_DIR = libft/obj

SRC_FILES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/exec.c

# vpath %.c src
# vpath %.h include

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

LIBFT_A = libft/libft.a

GREEN = \033[32m
RED = \033[31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT_A) -o $(NAME) -l readline
	@echo "\n$(GREEN)Compilation successful ✅ $(NAME)$(RESET)\n"

$(LIBFT_A): FORCE
	@$(MAKE) --no-print-directory -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/minishell.h Makefile 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) $(INC_LIBFT) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C libft clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) --no-print-directory -C libft fclean

re: fclean all

FORCE:
.PHONY: all re clean fclean bonus
