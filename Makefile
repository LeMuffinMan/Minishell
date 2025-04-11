# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/01 17:29:24 by oelleaum          #+#    #+#              #
#    Updated: 2025/04/11 17:23:17 by oelleaum         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BOLD_GREEN			=	\e[1;32m
BOLD_RED			=	\e[1;31m
BOLD_BLUE			=	\e[1;34m
BOLD_YELLOW			=	\e[1;33m
BOLD_PURPLE			=	\e[1;35m
BOLD_CYAN			=	\e[1;36m
STOP_COLOR			=	\e[0m

-include $(DEPS) $(DEPS_BONUS)
vpath %.c src/parsing/token:src/parsing/lexer:src/parsing/tree:src/builtins:src/utils:src/parsing:src
vpath %.h include:src/libft/include
vpath %.a src/libft/obj

CC					=	cc
FLAG				=	-Wall -Wextra -Werror -g3 -MMD -MP -I$(LIBFT_HEAD_DIR) -I$(HEAD_DIR)
NAME				=	minishell
HEAD				=	token.h list.h display.h lexer.h
HEAD_DIR			=	include/

LIBFT_DIR			=	src/libft/
LIBFT_HEAD_DIR		=	src/libft/include/
LIB_LIBFT			=	$(LIBFT_DIR)obj/libft.a
LIBFT_FLAG			=	-L$(LIBFT_DIR)src/ $(LIB_LIBFT)
LIBFT_HEAD			=	libft.h get_next_line.h ft_printf.h

SRC					=	main.c \
						parser.c \
						concat_args.c \
						create_tokenize_list.c \
						create_tree.c \
						display_list.c \
						display_tree.c \
						handle_cmd_and_path_utils.c \
						handle_cmd_and_path.c \
						tokenize_utils.c \
						tokenize.c \
						lexer.c \
						exec.c \
    				init.c \
    				builtins/echo.c \
    				builtins/cd.c \
    				builtins/pwd.c \
    				builtins/unset.c \
    				builtins/export.c \
    				builtins/env.c \
    				builtins/exit.c \
						utils/builtins_utils.c \
						utils/env_utils.c \
						utils/init_utils.c \
						utils/prints.c 

DEPS				=	$(SRC:%.c=$(OBJ_DIR)%.d)
OBJ					=	$(SRC:%.c=$(OBJ_DIR)%.o)
OBJ_DIR				=	.objs/

$(OBJ_DIR)%.o:%.c $(HEAD) $(LIBFT_HEAD) Makefile $(LIBFT_DIR)Makefile
	@mkdir -p $(@D) 
	@echo "$(BOLD_PURPLE)"
	$(CC) $(FLAG) -c $< -o $@
	@echo "$(STOP_COLOR)"

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(BOLD_BLUE)Compilling Libft...$(STOP_COLOR)"
	@make -C $(LIBFT_DIR)
	@echo "$(BOLD_GREEN)SUCCESS !!!$(STOP_COLOR)"
	@echo "$(BOLD_BLUE)Creating executable $(NAME)...$(BOLD_PURPLE)"
	$(CC) $(OBJ) $(PIPEX_FLAG) $(LIBFT_FLAG) -l readline -o $(NAME)
	@echo "$(STOP_COLOR)$(BOLD_GREEN)SUCCESS !!!$(STOP_COLOR)"

clean:
	@echo "$(BOLD_BLUE)Delete obj...$(STOP_COLOR)"
	@make clean -sC $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "$(BOLD_RED)SUCCESS !!!$(STOP_COLOR)"

fclean: clean
	@echo "$(BOLD_BLUE)Make fclean...$(STOP_COLOR)"
	@rm -f $(LIB_LIBFT) $(NAME) $(BONUS) here_doc
	@echo "$(BOLD_RED)SUCCESS !!!$(STOP_COLOR)"

re: fclean all

.PHONY: all clean fclean re bonus
