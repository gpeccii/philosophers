# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/26 12:04:19 by gpecci            #+#    #+#              #
#    Updated: 2023/05/29 16:21:59 by gpecci           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

MAKEFLAGS += --silent

SRCS =	philo.c \
		utils.c \
		init.c \
		eat.c \
		limit_cases.c \
		exit.c \

OBJS = ${SRCS:.c=.o}

CC		= gcc
RM		= rm -f
CFLAGS = -Wall -Wextra -Werror

.c.o:
		${CC} ${CFLAGS} -g -c $< -o ${<:.c=.o}

$(NAME):	$(OBJS)
			$(CC) $(OBJS) -o $(NAME)
	@echo "\033[32mCompiled OK!\033[0m"

all : $(NAME)

clean:
		${RM} ${OBJS}
	@echo "\033[33mclean OK!\033[0m"

fclean:		clean
			${RM} $(NAME)
	@echo "\033[33mfclean OK!\033[0m"

re:			fclean all
