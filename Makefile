C = gcc
LINC = ./includes/libft
PINC = ./includes/ft_printf
NAME = ft_ls
SRCS = ft_ls.c
OBJS = $(patsubst %.c, %.o, $(SRCS))
LIBD = -L$(LINC) -lft
PIBD = -L$(PINC) -lftprintf
CFLAGS = -g -Wall -Werror -Wextra -I$(LINC) -I$(PINC) 
OFLAGS = -o $(NAME) $(OBJS) $(LIBD) $(PIBD)
DSRCS = $(SRCS)
DFLAGS = $(CFLAGS) -g $(LIBD) $(DSRCS) -o
DNAME = $(NAME)_debug
DOBJS = $(patsubst %.c, %.o, $(DSRCS))

all: $(NAME)

libft.a : ./includes/libft/Makefile
	make -C ./includes/libft/
libftprintf.a : ./includes/ft_printf/Makefile
	make -C ./includes/ft_printf/

$(NAME): libft.a libftprintf.a $(OBJS)
	$(CC) $(OFLAGS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)
	echo "successful object compilation"

clean:
	make clean -C ./includes/libft/
	make clean -C ./includes/ft_printf/
	rm -f $(OBJS)

fclean: clean
	make fclean -C ./includes/libft/
	make fclean -C ./includes/ft_printf/
	rm -f $(NAME)
	rm -f $(DNAME)
	rm -rf $(DNAME).dSYM
re: fclean
	make
debug: $(DNAME)

$(DNAME): fclean libft.a
	$(CC) $(DFLAGS) $(DNAME)
	lldb $(DNAME) $(DARGS)
