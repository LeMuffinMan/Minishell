# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/01 17:29:24 by oelleaum          #+#    #+#              #
#    Updated: 2025/04/22 03:54:25 by asinsard         ###   ########lyon.fr    #
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
vpath %.c src/parsing/token:src/parsing/lexer:src/parsing/tree:src/builtins:src/utils:src/parsing:src/parsing/handle_quote:src/exec:src:src/ultrabonus:src/parsing/here_doc
vpath %.h include:include/parsing:include/exec:src/libft/include
vpath %.a src/libft/obj

CC					=	cc
FLAG				=	-Wall -Wextra -Werror -g3 -MMD -MP -I$(LIBFT_HEAD_DIR) $(INC_HEAD)
NAME				=	minishell
HEAD				=	here_doc.h lexer.h list.h quote.h token.h tree.h 
HEAD_DIR			=	include
INC_HEAD			=	-I$(HEAD_DIR) -I$(HEAD_DIR)/parsing -I$(HEAD_DIR)/exec

LIBFT_DIR			=	src/libft
LIBFT_HEAD_DIR		=	src/libft/include
LIB_LIBFT			=	$(LIBFT_DIR)/obj/libft.a
LIBFT_FLAG			=	-L$(LIBFT_DIR)/src/ $(LIB_LIBFT)
LIBFT_HEAD			=	libft.h get_next_line.h ft_printf.h

SRC					=	main.c \
						concat_args.c \
						create_here_doc.c \
						create_tokenize_list.c \
						create_tree.c \
						display_list.c \
						display_tree.c \
						handle_cmd_and_path_utils.c \
						handle_cmd_and_path.c \
						quote.c \
						tokenize_utils.c \
						tokenize.c \
						tree_utils.c \
						lexer.c \
						parser.c \
						init.c \
    				exec/builtins/echo.c \
    				exec/builtins/cd.c \
    				exec/builtins/pwd.c \
    				exec/builtins/unset.c \
    				exec/builtins/export.c \
    				exec/builtins/env.c \
    				exec/builtins/exit.c \
						utils/builtins_utils.c \
						utils/env_utils.c \
						utils/init_utils.c \
						utils/prints.c \
						exec/exec.c \
						exec/pipe.c \
						utils/misc.c \
						ultrabonus/get_prompt.c \
						ultrabonus/get_prompt_free.c \
						ultrabonus/git_branch_utils.c \
						ultrabonus/prompt_gets_fcts.c \
						ultrabonus/prompt_gets_fcts_utils.c  


DEPS				=	$(SRC:%.c=$(OBJ_DIR)%.d)
OBJ					=	$(SRC:%.c=$(OBJ_DIR)%.o)
OBJ_DIR				=	.objs/

$(OBJ_DIR)%.o:%.c $(HEAD) $(LIBFT_HEAD) Makefile $(LIBFT_DIR)/Makefile
	@mkdir -p $(@D) 
	@echo "$(BOLD_PURPLE)"
	$(CC) $(FLAG) -c $< -o $@
	@echo "$(STOP_COLOR)"

all: lib $(NAME)

lib:
	@echo "$(BOLD_BLUE)Compilling Libft...$(STOP_COLOR)"
	@make -C $(LIBFT_DIR)
	@echo "$(BOLD_GREEN)SUCCESS !!!$(STOP_COLOR)"

$(NAME): $(OBJ) $(LIB_LIBFT)
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
