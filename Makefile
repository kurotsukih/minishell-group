SRCS = 1_main.c 2_parse.c 3_dollar.c 4_exec.c 5_exec.c \
	   6_exec.c utils.c utils_cmd.c utils_env.c utils_str.c 
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