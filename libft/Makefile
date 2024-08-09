# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/19 17:01:53 by nkannan           #+#    #+#              #
#    Updated: 2024/05/19 09:01:57 by nkannan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CTYPE_SRCS		=	ctype/ft_isalnum.c ctype/ft_isalpha.c ctype/ft_isascii.c \
					ctype/ft_isdigit.c ctype/ft_isprint.c ctype/ft_tolower.c \
					ctype/ft_toupper.c ctype/ft_isdouble.c ctype/ft_isblank.c \
					ctype/ft_isspace.c ctype/ft_isinteger.c
STDIO_SRCS		=	stdio/ft_putchar_fd.c stdio/ft_putendl_fd.c \
					stdio/ft_putnbr_fd.c stdio/ft_putstr_fd.c
STDLIB_SRCS		=	stdlib/ft_atoi.c stdlib/ft_calloc.c stdlib/ft_itoa.c \
					stdlib/ft_atof.c stdlib/ft_strtol.c
STRING_SRCS		=	string/ft_bzero.c string/ft_memchr.c string/ft_memcmp.c \
					string/ft_memcpy.c string/ft_memmove.c string/ft_memset.c \
					string/ft_split.c string/ft_strchr.c string/ft_strdup.c \
					string/ft_striteri.c string/ft_strjoin.c string/ft_strlcat.c \
					string/ft_strlcpy.c string/ft_strlen.c string/ft_strmapi.c \
					string/ft_strncmp.c string/ft_strnstr.c string/ft_strrchr.c \
					string/ft_strtrim.c string/ft_substr.c \
					string/ft_strcmp.c
LIST_SRCS		=	list/ft_lstadd_back.c list/ft_lstadd_front.c \
					list/ft_lstclear.c list/ft_lstdelone.c \
					list/ft_lstiter.c list/ft_lstlast.c \
					list/ft_lstmap.c list/ft_lstnew.c list/ft_lstsize.c


SRCS			= $(CTYPE_SRCS) $(STDIO_SRCS) $(STDLIB_SRCS) $(STRING_SRCS) $(LIST_SRCS)

OBJS			= $(SRCS:.c=.o)

BONUS			= $(LIST_SRCS)
BONUS_OBJS		= $(BONUS:.c=.o)

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror

NAME			= libft.a

all:			$(NAME)

$(NAME):		$(OBJS)
				ar rcs $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

bonus:			$(OBJS) $(BONUS_OBJS)
				ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

.PHONY:			all clean fclean re bonus