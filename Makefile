CC = gcc
RM = rm -rf
NAME = MINILINUX

INCS = include
SRCS = source/directory.c source/instruction.c source/main.c source/stack.c source/time.c source/user.c source/cat.c source/cd.c source/chmod.c source/chown.c source/system.c source/find.c source/mkdir.c source/rm.c source/pwd.c source/ls.c source/cp.c source/permission.c source/grep.c
OBJS = $(SRCS:.c=.o)

.c.o: $(SRCS)
	$(CC) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -o $@ -I$(INCS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re