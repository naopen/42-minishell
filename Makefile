# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/08 17:13:55 by nkannan           #+#    #+#              #
#    Updated: 2024/12/02 16:41:17 by mkaihori         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS		= -Wall -Werror -Wextra -MMD -MP -g
CFLAGS		+= -I $(shell brew --prefix readline)/include
SANITIZE = -fsanitize=address -g

LIBFT = libft/libft.a
LIBFT_DIR = libft

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

SRCS = main.c \
       tokenizer.c \
       parser.c \
       expander.c \
       executor.c \
       builtin.c \
       env.c \
       utils.c \
	   tokenizer_utils.c \
	   error.c \
	   redirect.c \
	   heredoc.c \
	   free.c 

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline -L $(shell brew --prefix readline)/lib

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

debug: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(SANITIZE) $(OBJS) $(LIBFT) -o $(NAME) -lreadline -L $(shell brew --prefix readline)/lib
