# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by dde-jesu          #+#    #+#              #
#    Updated: 2019/02/22 17:12:06 by dde-jesu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell
CFLAGS=-Wall -Wextra -Iinclude -Ilibft/include -Ilibrl/include
CC=gcc

include src.mk

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile src.mk

$(NAME): $(OBJS)
	$(MAKE) -C libft libft.a
	$(MAKE) -C librl librl.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a librl/librl.a

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
