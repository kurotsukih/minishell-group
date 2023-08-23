SRCS = main.c \
	   util1.c \
	   utils_list.c \
	   utils4.c \
	   parsing1.c \
	   parsing2_check_tokens.c \
	   parsing4_expand_token.c \
	   parsing5_expand_string.c \
	   parsing6_expand_dollar_conversions.c \
	   execution/execute_builtins.c \
	   execution/execute_builtins2.c \
	   execution/preprocess.c \
	   execution/preprocess_cmd.c \
	   execution/prepare_pipe.c \
	   execution/execute_program.c \
	   execution/find_path.c \
	   execution/exec_1recursion.c \
	   execution/exec_2command.c

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