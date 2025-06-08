CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-deprecated-declarations -pedantic -std=gnu89
AR = ar
ARFLAGS = rcs
NAME = libhblk_blockchain_v0.1.a

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
