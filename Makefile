SRCS = 1_main.c 2_exec.c 3_exec.c 4_utils_parse.c 5_utils_exec.c \
       6_utils_env.c 7_utils_str.c 8_utils.c 
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