# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asinsard <asinsard@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/01 17:29:24 by oelleaum          #+#    #+#              #
#    Updated: 2025/05/09 14:11:44 by asinsard         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BOLD_GREEN			=	\e[1;32m
BOLD_RED			=	\e[1;31m
BOLD_BLUE			=	\e[1;34m
BOLD_YELLOW			=	\e[1;33m
BOLD_PURPLE			=	\e[1;35m
BOLD_CYAN			=	\e[1;36m
STOP_COLOR			=	\e[0m


DIR_EXEC			=	src \
						src/exec \
						src/exec/builtins \
						src/exec/builtins/builtins_utils \
						src/exec/exec_utils \
						src/init \
						src/misc

DIR_PARSING			=	src/parsing \
						src/parsing/expand \
						src/parsing/handle_quote \
						src/parsing/here_doc \
						src/parsing/lexer \
						src/parsing/token \
						src/parsing/tree

DIR_ULTRABONUS		=	src/ultrabonus \
						src/ultrabonus/get_prompt 

vpath %.c $(DIR_EXEC) $(DIR_PARSING) $(DIR_ULTRABONUS)
vpath %.h include:include/parsing:include/exec:src/libft/include
vpath %.a src/libft/obj

CC					=	cc
FLAGS				=	-Wall -Wextra -Werror -g3 -MMD -MP -I$(LIBFT_HEAD_DIR) $(INC_HEAD)
NAME				=	minishell

HEAD				=	display.h $(HEAD_EXEC)

HEAD_DIR			=	include

INC_HEAD			=	-I$(HEAD_DIR) -I$(HEAD_DIR)/parsing -I$(HEAD_DIR)/exec -I$(HEAD_DIR)/ultrabonus

LIBFT_DIR			=	src/libft
LIBFT_HEAD_DIR		=	src/libft/include
LIB_LIBFT			=	$(LIBFT_DIR)/obj/libft.a
LIBFT_FLAG			=	-L$(LIBFT_DIR)/src/ $(LIB_LIBFT)
LIBFT_HEAD			=	libft.h get_next_line.h ft_printf.h

SRC					=	main.c \
						concat_args.c \
						concat_args_utils.c \
						create_here_doc.c \
						create_tokenize_list.c \
						create_tree.c \
						display_list.c \
						display_tree.c \
						expand_utils.c \
						expand.c \
						handle_cmd_and_path_utils.c \
						handle_cmd_and_path.c \
						handle_space.c \
						handle_utils.c \
						quote.c \
						tokenize_utils.c \
						tokenize.c \
						tree_utils.c \
						lexer.c \
						lexer_utils.c \
						parser.c \
						init/init.c \
					signals.c \
					exec.c \
					errors.c \
    				echo.c \
    				cd.c \
    				pwd.c \
    				unset.c \
    				export.c \
    				env.c \
    				exit.c \
					redirections_stdio_utils.c \
					pipe_utils.c \
					export_utils.c \
					builtins_utils.c \
					env_utils.c \
					init_utils.c \
					prints.c \
					misc.c \
					source.c \
					get_prompt.c \
					get_prompt_free.c \
					git_branch_utils.c \
					load_minishellrc.c \
					prompt_gets_fcts.c \
					prompt_gets_fcts_utils.c \
					history.c \
					aliases.c \
					shell_functions.c


DEPS				=	$(SRC:%.c=$(OBJ_DIR)%.d)
OBJ					=	$(SRC:%.c=$(OBJ_DIR)%.o)
OBJ_DIR				=	.objs/

$(OBJ_DIR)%.o:%.c $(LIB_LIBFT) Makefile
	@mkdir -p $(@D) 
	@echo "$(BOLD_YELLOW)Compilling" $@ "$(BOLD_PURPLE)"
	@$(CC) $(FLAGS) -c $< -o $@
	@echo -n "$(STOP_COLOR)"

all: lib $(NAME)

debug:
	@echo "				$(BOLD_BLUE)DEBUG"
	@echo "$(STOP_COLOR)"
	@make re --no-print-directory FLAGS="$(FLAGS) -DDEBUG=1"

lib:
	@echo "$(BOLD_BLUE)Compilling Libft...$(STOP_COLOR)"
	@make --no-print-directory -C $(LIBFT_DIR)
	@echo "$(BOLD_GREEN)SUCCESS !!!$(STOP_COLOR)"

$(NAME): $(OBJ) $(LIB_LIBFT)
	@echo "$(BOLD_BLUE)Creating executable $(NAME)...$(BOLD_PURPLE)"
	$(CC) $(OBJ) "-DDEBUG=0" $(LIBFT_FLAG) -l readline -o $(NAME) 
	@echo "$(STOP_COLOR)$(BOLD_GREEN)SUCCESS !!!$(STOP_COLOR)"


clean:
	@echo "$(BOLD_BLUE)Delete obj...$(STOP_COLOR)"
	@make clean --no-print-directory -sC $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "$(BOLD_RED)SUCCESS !!!$(STOP_COLOR)"

fclean: clean
	@echo "$(BOLD_BLUE)Make fclean...$(STOP_COLOR)"
	@rm -f $(LIB_LIBFT) $(NAME) $(BONUS) here_doc
	@echo "$(BOLD_RED)SUCCESS !!!$(STOP_COLOR)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus debug
