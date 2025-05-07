BOLD_GREEN		=	\e[1;32m
BOLD_RED		=	\e[1;31m
BOLD_BLUE		=	\e[1;34m
BOLD_YELLOW		=	\e[1;33m
PURPLE			=	\e[1;35m
BOLD_CYAN		=	\e[1;36m
STOP_COLOR		=	\e[0m

CC 				=	cc
CFLAGS 			=	-Wall -Wextra -Werror -g -MMD -MP -Iinclude
NAME			=	$(OBJ_DIR)libft.a
HEAD			=	libft.h

-include $(DEPS)
vpath %.h include
vpath %.c src:src/get_next_line:src/ft_printf

SRCMANDATORY	=	free_tab.c \
					ft_atoi.c \
					ft_bzero.c \
					ft_calloc.c \
					ft_error.c \
					ft_isalnum.c \
					ft_isalpha.c \
					ft_isascii.c \
					ft_isdigit.c \
					ft_isprint.c \
					ft_isslash.c \
					ft_isspace.c \
					ft_itoa.c \
					ft_memchr.c \
					ft_memcmp.c \
					ft_memcpy.c \
					ft_memmove.c \
					ft_memset.c \
					ft_putchar_fd.c \
					ft_putendl_fd.c \
					ft_putnbr_fd.c \
					ft_putstr_fd.c \
					ft_split.c \
					ft_str_isspace.c \
					ft_strchr.c \
					ft_strdup.c \
					ft_striteri.c \
					ft_strjoin.c \
					ft_strlcat.c \
					ft_strlcpy.c \
					ft_strlen.c \
					ft_strmapi.c \
					ft_strcmp.c \
					ft_strncmp.c \
					ft_strndup.c \
					ft_strnstr.c \
					ft_strrchr.c \
					ft_strtrim.c \
					ft_substr.c \
					ft_tab_len.c \
					ft_tolower.c \
					ft_toupper.c \
					ft_printf.c \
					ft_print_num.c \
					ft_print_utils.c \
					get_next_line.c \
					get_next_line_utils.c

SRCBONUS		=	ft_lstadd_back_bonus.c \
					ft_lstadd_front_bonus.c \
					ft_lstclear_bonus.c \
					ft_lstdelone_bonus.c \
					ft_lstiter_bonus.c \
					ft_lstlast_bonus.c \
					ft_lstmap_bonus.c \
					ft_lstnew_bonus.c \
					ft_lstsize_bonus.c
				

OBJ_DIR 		=	obj/
OBJ				=	$(SRCMANDATORY:%.c=$(OBJ_DIR)%.o)
DEPS			=	$(SRCMANDATORY:%.c=$(OBJ_DIR)%.d)

$(OBJ_DIR)%.o:%.c $(HEAD) Makefile
	@mkdir -p $(OBJ_DIR)
	@echo "$(BOLD_CYAN)" $@ " $(PURPLE)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -n "$(STOP_COLOR)"

all: $(NAME)

$(NAME): $(OBJ)
	@ar -rcs $(NAME) $(OBJ)

bonus:
	$(MAKE) SRCMANDATORY="$(SRCMANDATORY) $(SRCBONUS)"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
