SRCS = main.c \
	   util1.c \
	   util2_clean.c \
	   parsing/ft_add_spaces.c \
	   parsing/ft_expand_string.c \
	   parsing/ft_expand_token.c \
	   parsing/ft_check_tokens.c \
	   parsing/ft_assign_types.c \
	   parsing/ft_open_heredocs.c \
	   parsing/ft_make_tree.c \
	   execution/ft_preprocess.c \
	   execution/preprocess/ft_preprocess_cmd.c \
	   execution/preprocess/ft_preprocess_parameter.c \
	   execution/preprocess/ft_preprocess_redirection.c \
	   execution/preprocess/ft_preprocess_utils.c \
	   execution/ft_prepare_pipe.c \
	   execution/ft_execute_program.c \
	   execution/execute_program/ft_find_path.c \
	   execution/execute_builtin.c \
	   execution/execute_builtins_1.c \
	   execution/execute_builtins_2.c \
	   execution/execute_builtins_3.c \
	   ft_exec_1recursion.c \
	   ft_exec_2command.c \

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