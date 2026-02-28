# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/18 21:38:29 by shunwata          #+#    #+#              #
#    Updated: 2026/02/28 21:39:40 by nmasuda          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -I $(INC_DIR) -I $(L_INC_DIR) -I $(PRINTF_DIR) -I $(GNL_DIR) -I $(ADDITIONAL_DIR)
LIBS			= -lreadline -lhistory

INC_DIR			= includes
SRC_DIR			= srcs
BUILTIN_DIR		= $(SRC_DIR)/builtin
B_UTILS_DIR		= $(BUILTIN_DIR)/builtin_utils

LIBFT			= $(LIBFT_DIR)/libft.a
LIBFT_DIR		= Libft
L_INC_DIR		= $(LIBFT_DIR)/includes
L_SRC_DIR		= $(LIBFT_DIR)/srcs
PRINTF_DIR		= $(L_SRC_DIR)/ft_printf
GNL_DIR			= $(L_SRC_DIR)/get_next_line
ADDITIONAL_DIR	= $(L_SRC_DIR)/additional

MAIN_SRC		= $(addprefix $(SRC_DIR)/, core_executor.c core_parser.c \
					core_tokenizer.c executor_builtin.c expander.c \
					expander_env.c expander_quotes.c get_fullpath.c \
					heredoc.c init.c main.c parser_redir.c puterr.c \
					signal.c utils_common.c utils_executor.c \
					utils_heredoc.c utils_parser.c utils_parser_is.c \
					utils_tokenizer.c utils_tokenizer2.c)

BUILTIN_SRC		= $(addprefix $(BUILTIN_DIR)/, builtin_cd.c builtin_echo.c builtin_env.c \
					builtin_exit.c builtin_export.c builtin_pwd.c builtin_unset.c)

B_UTILS_SRC		= $(addprefix $(B_UTILS_DIR)/, utils_export.c utils_export_append.c)

SRCS			= $(MAIN_SRC) $(BUILTIN_SRC) $(B_UTILS_SRC)

OBJS			= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=leak.supp -q ./$(NAME)

.PHONY: all clean fclean re
