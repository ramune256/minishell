# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/18 21:38:29 by shunwata          #+#    #+#              #
#    Updated: 2025/12/04 13:43:56 by nmasuda          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I $(INC_DIR) -I $(L_INC_DIR) -I $(PRINTF_DIR) -I $(GNL_DIR)
LIBS		= -lreadline -lhistory

INC_DIR		= includes
SRC_DIR		= srcs
BUILTIN_DIR	= $(SRC_DIR)/builtin
B_UTILS_DIR	= $(BUILTIN_DIR)/builtin_utils

LIBFT		= $(LIBFT_DIR)/libft.a
LIBFT_DIR	= Libft
L_INC_DIR	= $(LIBFT_DIR)/includes
L_SRC_DIR	= $(LIBFT_DIR)/srcs
PRINTF_DIR	= $(L_SRC_DIR)/ft_printf
GNL_DIR		= $(L_SRC_DIR)/get_next_line

MAIN_SRC	= $(addprefix $(SRC_DIR)/, core_executor.c core_parser.c \
				core_tokenizer.c debug.c executor_builtin.c executor_heredoc.c \
				get_fullpath.c init.c main.c parser_redir.c put_err.c signal.c split_improved.c \
				utils_common.c utils_executor.c utils_parser.c utils_tokenizer.c)

BUILTIN_SRC	= $(addprefix $(BUILTIN_DIR)/, builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c \
				builtin_export.c builtin_main.c builtin_pwd.c builtin_unset.c)

B_UTILS_SRC	= $(addprefix $(B_UTILS_DIR)/, utils_check.c utils_error.c utils_export.c utils_free_all.c \
				utils_ft_atol.c utils_ft_export_strjoin.c utils_search.c)

SRCS		= $(MAIN_SRC) $(BUILTIN_SRC) $(B_UTILS_SRC)

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
