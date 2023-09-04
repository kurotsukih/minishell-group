SRCS = 1_main.c 2_parse.c 3_exec.c 4_builtins.c 5_builtins.c \
       6_utils.c 8_utils_exec.c 9_utils_env.c 10_utils_str.c
OBJS = $(addprefix ./.build/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)

all : minishell

minishell : ${OBJS}
	cc $(OBJS) -o minishell -L /usr/local/opt/readline/lib -lreadline -Llibft -lft

./.build/%.o : ./srcs/%.c ./libft/libft.a
	@mkdir ./.build/ 2> /dev/null || true
	cc -g3 -Wall -Werror -Wextra -I ./srcs -I ./libft -I /usr/local/opt/readline/include -c $< -o $@

./libft/libft.a	:
	make -C ./libft

clean :
	make -C ./libft clean
	rm -rf ${OBJS}

fclean : clean
	rm -rf minishell ./libft/libft.a

re : fclean all

.PHONY : all clean fclean re