#############################################################################################
#                                                                                           #
#                                           MAKEFILE                                        #
#                                                                                           #
#############################################################################################

NAME	= minishell

LIBFT	= make -C ./libft

SRC_DIR	= srcs/

OBJ_DIR	= objs/

FILES	=	1_main.c \
			2_parse_and_opens_fds.c \
			3_exec.c 4_builtins.c \
			5_utils_parse.c \
			6_utils_exec.c \
			7_utils_lst.c \
			8_utils.c 

SRCS	= $(addprefix $(SRC_DIR), $(FILES))

OBJS	= $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

FLAGS	= -Wall -Wextra -Werror -g3

LIB		= -Llibft -lft -lreadline # -L /usr/local/opt/readline/lib

INCL	= -I ./includes/ -I ./libft # -I /usr/local/opt/readline/include


objs/%.o : ./srcs/%.c
			mkdir -p $(OBJ_DIR)
			cc $(FLAGS) $(INCL) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(LIBFT)
			cc $(OBJS) -o $(NAME) $(LIB)

clean:
			make -C ./libft clean
			rm -rf ${OBJ_DIR}

fclean: 	clean
			rm -rf $(NAME) ./libft/libft.a

re: 		fclean all

.PHONY : 	all clean fclean re
