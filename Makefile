SRCS = main.c \
	   minishell1.c \
	   minishell2.c \
	   minishell4_exec.c \
	   minishell5_exec.c \
	   utils1.c \
	   utils2.c \
	   utils3_list.c \
	   prepare_pipe.c \
	   find_path.c \
#	   minishell3_exec.c \

OBJS = $(addprefix ./.build/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)
FLAGS = -Wall -Werror -Wextra -I ./srcs -I ./libft -I /usr/local/opt/readline/include

all : minishell

valgrind : minishell
	valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --suppressions=./ignore.txt ./minishell

minishell : ${OBJS}
	cc $(OBJS) -o minishell -L /usr/local/opt/readline/lib -lreadline -Llibft -lft


./.build/%.o		: ./srcs/%.c ./libft/libft.a
	cc -g3 $(FLAGS) -c $< -o $@ 

./libft/libft.a	:
	make -C ./libft

clean :
	make -C ./libft clean
	rm -rf ${OBJS}

fclean : clean
	rm -rf minishell ./libft/libft.a

test_add_spaces :
	cc -g3 $(FLAGS) tests/test_add_spaces.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_UTILS)) -lreadline -Llibft -lft
	./a.out
	rm a.out

re : fclean all

git :
	git add *
	git commit -m "-"
	git push

.PHONY : all clean fclean re