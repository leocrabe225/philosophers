SRCS = philosophers.c philosophers_2.c

OBJS = ${SRCS:.c=.o}

NAME = philosophers

FLAGS = -Werror -Wall -Wextra -pthread

CC = gcc

RM = rm -rf

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o} 

${NAME}: ${OBJS}
	${CC} -o ${NAME} -pthread ${OBJS}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean:	clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re .c.o
