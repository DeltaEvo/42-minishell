# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by dde-jesu          #+#    #+#              #
#    Updated: 2018/12/17 13:41:47 by dde-jesu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell
CFLAGS=-Wall -Wextra -Werror -Iinclude -Ilibft/include -Ilibrl/include -g
CC=gcc

include src.mk

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile src.mk

$(NAME): $(OBJS)
	$(MAKE) -C libft libft.a
	$(MAKE) -C librl librl.a
	$(CC) -o $(NAME) $(OBJS) libft/libft.a librl/librl.a

clean:
	$(MAKE) -C libft clean
	$(MAKE) -C librl clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	$(MAKE) -C librl fclean
	rm -rf $(NAME)

re: fclean $(NAME)


.PHONY: clean fclean re proto
