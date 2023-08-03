SRCS_ADD_SPACES		=	parsing/ft_add_spaces.c \
						parsing/utils/ft_add_spaces_utils.c \
						parsing/utils/ft_add_spaces_utils_2.c
SRCS_TOKENIZATION	=	parsing/ft_tokenization.c \
						parsing/tokenization/ft_add_token.c \
						parsing/tokenization/ft_is_token.c \
						parsing/tokenization/ft_tokenization_utils.c \
						parsing/tokenization/ft_remove_quotes.c
SRCS_EXPANSION		=	parsing/expansion/ft_expand_string.c \
						parsing/expansion/ft_expand_token.c \
						parsing/expansion/utils/ft_expand_string_utils.c \
						parsing/expansion/utils/ft_expand_string_utils_2.c \
						parsing/expansion/utils/ft_expand_token_utils.c \
						parsing/expansion/utils/ft_expand_token_utils_2.c \
						parsing/expansion/utils/ft_expand_token_utils_3.c
SRCS_ASSIGN_TYPES	=	parsing/ft_check_tokens.c \
						parsing/check_tokens/ft_check_tokens_utils.c \
						parsing/ft_assign_types.c
SRCS_HEREDOCS		=	parsing/ft_open_heredocs.c
SRCS_MAKE_TREE		=	parsing/ft_make_tree.c \
						parsing/make_tree/ft_make_tree_utils.c
SRCS_PARSING		=	ft_parsing.c $(SRCS_ADD_SPACES) $(SRCS_TOKENIZATION) $(SRCS_EXPANSION) $(SRCS_ASSIGN_TYPES) $(SRCS_WILDCARDS) $(SRCS_HEREDOCS) $(SRCS_MAKE_TREE)

SRCS_UTILS			=	utils/ft_clean_1.c \
						utils/ft_clean_2.c \
						utils/ft_error_1.c \
						utils/ft_error_2.c

SRCS_PREPROCESS		=	execution/ft_preprocess.c \
						execution/preprocess/ft_preprocess_cmd.c \
						execution/preprocess/ft_preprocess_parameter.c \
						execution/preprocess/ft_preprocess_redirection.c \
						execution/preprocess/ft_preprocess_utils.c

SRCS_PREPARE_PIPE	=	execution/ft_prepare_pipe.c

SRCS_EXEC_PROGRAM	=	execution/ft_execute_program.c \
						execution/execute_program/ft_construct_command.c \
						execution/execute_program/ft_find_path.c \
						execution/execute_program/utils/ft_find_path_utils.c \
						execution/execute_program/utils/ft_split_alt2.c \
						execution/execute_program/utils/ft_exec_command_utils.c

SRCS_EXEC_BUILTIN	=	execution/ft_execute_builtin.c \
						execution/execute_builtin/ft_builtin_cd.c \
						execution/execute_builtin/ft_builtin_echo.c \
						execution/execute_builtin/ft_builtin_env.c \
						execution/execute_builtin/ft_builtin_exit.c \
						execution/execute_builtin/ft_builtin_export.c \
						execution/execute_builtin/utils/ft_builtin_export_utils.c \
						execution/execute_builtin/ft_builtin_pwd.c \
						execution/execute_builtin/ft_builtin_unset.c

SRCS_EXECUTION		=	ft_exec_1recursion.c utils/ft_exec_recursion_utils.c ft_exec_2command.c $(SRCS_PREPROCESS) $(SRCS_PREPARE_PIPE) $(SRCS_EXEC_PROGRAM) $(SRCS_EXEC_BUILTIN)

SRCS				=	main.c init/ft_signal.c $(SRCS_PARSING) $(SRCS_UTILS) $(SRCS_INIT) $(SRCS_EXECUTION)
OBJS = $(addprefix ./.build/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)
FLAGS = -Wall -Werror -Wextra -I ./includes -I /usr/local/opt/readline/include

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


test_add_spaces : debug_add_spaces
	./a.out
	rm a.out

test_tokenization : debug_tokenization
	./a.out
	rm a.out

test_assign_types : debug_assign_types
	./a.out
	rm a.out

test_make_tree : debug_make_tree
	./a.out
	rm a.out

test_preprocess : debug_preprocess
	./a.out
	rm a.out

test_exec_recursion : debug_exec_recursion
	./a.out
	rm a.out

test_execute : debug_execute
	./a.out
	rm a.out

test_exec_program : debug_exec_program
	./a.out
	rm a.out

debug_add_spaces :
	cc -g3 $(FLAGS) tests/test_add_spaces.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_UTILS)) -lreadline -Llibft -lft

debug_tokenization :
	cc -g3 $(FLAGS) tests/test_tokenization.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_EXPANSION) $(SRCS_TOKENIZATION) $(SRCS_UTILS)) -lreadline -Llibft -lft

debug_assign_types :
	cc -g3 $(FLAGS) tests/test_assgn_types.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_EXPANSION) $(SRCS_TOKENIZATION) $(SRCS_ASSIGN_TYPES) $(SRCS_UTILS)) -lreadline -Llibft -lft

debug_make_tree :
	cc -g3 $(FLAGS) tests/test_make_tree.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_EXPANSION) $(SRCS_TOKENIZATION) $(SRCS_ASSIGN_TYPES) $(SRCS_MAKE_TREE) $(SRCS_UTILS)) -lreadline -Llibft -lft

debug_preprocess :
	cc -g3 $(FLAGS) tests/test_preprocess.c $(addprefix ./srcs/, $(SRCS_PARSING) $(SRCS_UTILS) $(SRCS_INIT) $(SRCS_PREPROCESS)) -lreadline -Llibft -lft

debug_exec_recursion :
	cc -g3 $(FLAGS) tests/test_exec_recursion.c $(addprefix ./srcs/, $(SRCS_ADD_SPACES) $(SRCS_EXPANSION) $(SRCS_TOKENIZATION) $(SRCS_ASSIGN_TYPES) $(SRCS_MAKE_TREE) $(SRCS_UTILS)) srcs/execution/ft_preprocess_node.c srcs/execution/ft_preprocess_tree.c srcs/ft_execution.c -lreadline -Llibft -lft	

debug_execute :
	cc -g3 $(FLAGS) tests/test_execute.c  $(addprefix ./srcs/, ft_exec_2command.c $(SRCS_UTILS) $(SRCS_EXEC_PROGRAM)) -lreadline -Llibft -lft

debug_exec_program :
	cc -g3 $(FLAGS) tests/test_exec_command.c  $(addprefix ./srcs/, ft_exec_2command.c $(SRCS_UTILS) $(SRCS_EXEC_PROGRAM) $(SRCS_PREPARE_PIPE)) -lreadline -Llibft -lft

re : fclean all

git :
	git add *
	git commit -m "-"
	git push

.PHONY : all clean fclean re